#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "landingpage.h"
#include <tapplication.h>
#include <tcsdtools.h>
#include <thelpmenu.h>
#include <tjobmanager.h>
#include <tpopover.h>
#include <twindowtabberbutton.h>
#include <widgets/zzzworkspaceeditor.h>

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
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionExit_triggered() {
    QApplication::exit();
}

void MainWindow::on_actionNew_Workspace_triggered() {
    auto* browser = new ZzzWorkspaceEditor(this);
    auto* initialBrowserTab = new tWindowTabberButton(QIcon(), tr("New Workspace"), ui->stackedWidget, browser);

    ui->stackedWidget->addWidget(browser);
    //    connect(browser, &ZzzWorkspaceEditor::titleChanged, this, [=] {
    //        initialBrowserTab->setText(browser->title());
    //    });
    //    connect(browser, &ZzzWorkspaceEditor::repositoryChanged, this, &MainWindow::updateMenuItems);

    ui->windowTabber->addButton(initialBrowserTab);
    d->tabButtons.insert(browser, initialBrowserTab);

    ui->stackedWidget->addWidget(browser);
}
