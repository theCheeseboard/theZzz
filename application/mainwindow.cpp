#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "landingpage.h"
#include <QCoroCore>
#include <QFileDialog>
#include <QStandardPaths>
#include <tapplication.h>
#include <tcsdtools.h>
#include <thelpmenu.h>
#include <tjobmanager.h>
#include <tpopover.h>
#include <twindowtabberbutton.h>
#include <widgets/zzzworkspaceeditor.h>
#include <workspacefile.h>
#include <zzzreplymanager.h>

struct MainWindowPrivate {
        tCsdTools csd;

        QMap<ZzzWorkspaceEditor*, tWindowTabberButton*> tabButtons;
};

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    d = new MainWindowPrivate();
    d->csd.installMoveAction(ui->topWidget);
    d->csd.installResizeAction(this);

    if (tCsdGlobal::windowControlsEdge() == tCsdGlobal::Left) {
        ui->leftCsdLayout->addWidget(d->csd.csdBoxForWidget(this));
    } else {
        ui->rightCsdLayout->addWidget(d->csd.csdBoxForWidget(this));
    }

    ui->jobButtonLayout->addWidget(tJobManager::makeJobButton());

    this->resize(this->size());

#ifdef Q_OS_MAC
    ui->menubar->addMenu(new tHelpMenu(this));
    ui->menuButton->setVisible(false);
#else
    ui->menubar->setVisible(false);
    QMenu* menu = new QMenu(this);

    tHelpMenu* helpMenu = new tHelpMenu(this);

    menu->addMenu(helpMenu);
    menu->addAction(ui->actionExit);

    ui->menuButton->setIcon(tApplication::applicationIcon());
    ui->menuButton->setIconSize(SC_DPI_T(QSize(24, 24), QSize));
    ui->menuButton->setMenu(menu);
#endif

    auto landingPage = new LandingPage(this);
    connect(landingPage, &LandingPage::newWorkspace, this, [this] {
        ui->actionNew_Workspace->trigger();
    });
    connect(landingPage, &LandingPage::openWorkspace, this, [this] {
        ui->actionOpen_Workspace->trigger();
    });

    ui->stackedWidget->setDefaultWidget(landingPage);
    ui->stackedWidget->setCurrentAnimation(tStackedWidget::SlideHorizontal);
    this->setWindowIcon(tApplication::applicationIcon());

    ui->replyViewer->setFixedWidth(400);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionExit_triggered() {
    QApplication::exit();
}

void MainWindow::on_actionNew_Workspace_triggered() {
    this->newTab();
}

void MainWindow::on_actionSave_triggered() {
    auto browser = qobject_cast<ZzzWorkspaceEditor*>(ui->stackedWidget->currentWidget());
    if (!browser) return;

    if (browser->currentFile().isEmpty()) {
        this->on_actionSave_As_triggered();
        return;
    }

    browser->saveWorkspace(browser->currentFile());
}

QCoro::Task<> MainWindow::on_actionSave_As_triggered() {
    auto browser = qobject_cast<ZzzWorkspaceEditor*>(ui->stackedWidget->currentWidget());
    if (!browser) co_return;

    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    dialog.setNameFilters({tr("theZzz Workspace (*.zzz)")});
    dialog.open();

    const auto result = co_await qCoro(&dialog, &QFileDialog::finished);
    if (result == QFileDialog::Rejected) co_return;

    QString path = dialog.selectedFiles().constFirst();
    browser->saveWorkspace(path);
}

QCoro::Task<> MainWindow::on_actionOpen_Workspace_triggered() {
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    dialog.setNameFilters({tr("theZzz Workspace (*.zzz)")});
    dialog.open();

    const auto result = co_await qCoro(&dialog, &QFileDialog::finished);
    if (result == QFileDialog::Rejected) co_return;

    QString path = dialog.selectedFiles().constFirst();
    auto browser = this->newTab();

    browser->openWorkspace(path);
}

ZzzWorkspaceEditor* MainWindow::newTab() {
    auto* browser = new ZzzWorkspaceEditor(this);
    auto* initialBrowserTab = new tWindowTabberButton(QIcon(), tr("New Workspace"), ui->stackedWidget, browser);

    ui->stackedWidget->addWidget(browser);
    connect(browser, &ZzzWorkspaceEditor::addReply, ui->replyViewer->replyManager().data(), &ZzzReplyManager::pushReply);
    connect(browser->workspace().data(), &WorkspaceFile::dataChanged, this, [browser, initialBrowserTab] {
        initialBrowserTab->setText(browser->workspace()->workspaceFileTitle());
    });

    ui->windowTabber->addButton(initialBrowserTab);
    d->tabButtons.insert(browser, initialBrowserTab);

    ui->stackedWidget->addWidget(browser);
    return browser;
}
