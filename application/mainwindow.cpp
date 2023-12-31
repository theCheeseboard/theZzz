#include <popovers/snapinpopover.h>
#include <popovers/snapins/checkoutsnapin.h>
#include <popovers/snapins/commitsnapin.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "commandpalette/commandpaletterequestsscope.h"
#include "landingpage.h"
#include <QCoroCore>
#include <QFileDialog>
#include <QStandardPaths>
#include <tapplication.h>
#include <tcommandpalette/tcommandpaletteactionscope.h>
#include <tcommandpalette/tcommandpalettecontroller.h>
#include <tcommandpalette/tcommandpalettedocumentspecificscope.h>
#include <tcsdtools.h>
#include <thelpmenu.h>
#include <tjobmanager.h>
#include <tmessagebox.h>
#include <tpopover.h>
#include <twindowtabberbutton.h>
#include <widgets/zzzworkspaceeditor.h>
#include <workspacefile.h>
#include <zzzreplymanager.h>
#include <zzzrequest.h>

struct MainWindowPrivate {
        tCsdTools csd;

        QMap<ZzzWorkspaceEditor*, tWindowTabberButton*> tabButtons;
        ZzzRequestTreeItemPtr currentRequest;

        tCommandPaletteDocumentSpecificScope* requestsScope;
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

    tCommandPaletteActionScope* commandPaletteActionScope;
    auto commandPalette = tCommandPaletteController::defaultController(this, &commandPaletteActionScope);

    d->requestsScope = new tCommandPaletteDocumentSpecificScope();
    commandPalette->addScope(d->requestsScope);

    ui->menubar->addMenu(new tHelpMenu(this, commandPalette));

#ifdef Q_OS_MAC
    ui->menuButton->setVisible(false);
#else
    ui->menubar->setVisible(false);
    QMenu* menu = new QMenu(this);

    tHelpMenu* helpMenu = new tHelpMenu(this);

    menu->addAction(ui->actionNew_Workspace);
    menu->addAction(ui->actionOpen_Workspace);
    menu->addSeparator();
    menu->addAction(ui->actionSave);
    menu->addAction(ui->actionSave_As);
    menu->addSeparator();
    menu->addMenu(ui->menuGit);
    menu->addSeparator();
    menu->addMenu(ui->menuView);
    menu->addAction(ui->actionClose_Tab);
    menu->addSeparator();
    menu->addAction(commandPalette->commandPaletteAction());
    menu->addMenu(helpMenu);
    menu->addAction(ui->actionExit);

    ui->menuButton->setIcon(tApplication::applicationIcon());
    ui->menuButton->setIconSize(QSize(24, 24));
    ui->menuButton->setMenu(menu);
#endif
    commandPaletteActionScope->addMenuBar(ui->menubar);

#ifdef Q_OS_MAC
    setupMacOs();
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

    connect(ui->stackedWidget, &tStackedWidget::switchingFrame, this, &MainWindow::updateMenuItems);
    updateMenuItems();

    ui->replyViewer->setFixedWidth(400);
    ui->replyViewer->setVisible(false);
    ui->line->setVisible(false);
}

MainWindow::~MainWindow() {
#ifdef Q_OS_MAC
    destroyMacOs();
#endif
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
    connect(browser, &ZzzWorkspaceEditor::addReply, this, [this](ZzzReplyPtr reply) {
        ui->actionResponses->setChecked(true);
        ui->replyViewer->replyManager()->pushReply(reply);
    });
    connect(browser, &ZzzWorkspaceEditor::currentRequestChanged, this, &MainWindow::updateContext);
    connect(browser->workspace().data(), &WorkspaceFile::dataChanged, this, [browser, initialBrowserTab, this] {
        initialBrowserTab->setText(browser->workspace()->workspaceFileTitle());

        auto currentEnvironment = browser->workspace()->currentEnvironment();
        initialBrowserTab->setSupplementaryText(browser->workspace()->environmentName(currentEnvironment));
        this->updateContext();
    });

    ui->windowTabber->addButton(initialBrowserTab);
    d->tabButtons.insert(browser, initialBrowserTab);

    auto requestsScope = new CommandPaletteRequestsScope(browser, this);
    connect(requestsScope, &CommandPaletteRequestsScope::showWorkspace, this, [browser, this] {
        ui->stackedWidget->setCurrentWidget(browser);
    });
    d->requestsScope->registerScope(browser, requestsScope);

    ui->stackedWidget->addWidget(browser);
    return browser;
}

QCoro::Task<> MainWindow::on_actionCommit_triggered() {
    auto repo = co_await currentFileGitRepository();
    if (!repo) co_return;

    SnapInPopover::showSnapInPopover(this, new CommitSnapIn(repo));
}

QCoro::Task<> MainWindow::on_actionPush_triggered() {
    auto repo = co_await currentFileGitRepository();
    if (!repo) co_return;

    SnapInPopover::showPushPopover(this, repo);
}

QCoro::Task<> MainWindow::on_actionPull_triggered() {
    auto repo = co_await currentFileGitRepository();
    if (!repo) co_return;

    SnapInPopover::showPullPopover(this, repo);
}

QCoro::Task<RepositoryPtr> MainWindow::currentFileGitRepository() {
    auto browser = qobject_cast<ZzzWorkspaceEditor*>(ui->stackedWidget->currentWidget());
    if (!browser) co_return {};

    auto repository = Repository::repositoryForDirectory(browser->currentFile());
    if (!repository) {
        tMessageBox box(this->window());
        box.setTitleBarText(tr("No Git Repository Available"));
        box.setMessageText(tr("This workspace file is not in a Git repository. Init a Git repository, and then try again."));
        box.setIcon(QMessageBox::Information);

        auto button = co_await box.presentAsync();
        co_return {};
    }
    co_return repository;
}

QCoro::Task<> MainWindow::on_actionCheckout_triggered() {
    auto repo = co_await currentFileGitRepository();
    if (!repo) co_return;

    SnapInPopover::showSnapInPopover(this, new CheckoutSnapIn(repo));
}

void MainWindow::on_actionClose_Tab_triggered() {
    auto browser = qobject_cast<ZzzWorkspaceEditor*>(ui->stackedWidget->currentWidget());
    if (browser) {
        ui->stackedWidget->removeWidget(browser);
        ui->windowTabber->removeButton(d->tabButtons.value(browser));
        d->tabButtons.remove(browser);
        browser->deleteLater();

        updateMenuItems();
    }
}

void MainWindow::updateMenuItems() {
    bool enabled = qobject_cast<ZzzWorkspaceEditor*>(ui->stackedWidget->currentWidget());

    ui->actionSave->setEnabled(enabled);
    ui->actionSave_As->setEnabled(enabled);
    ui->actionCommit->setEnabled(enabled);
    ui->actionCheckout->setEnabled(enabled);
    ui->actionPush->setEnabled(enabled);
    ui->actionPull->setEnabled(enabled);
    ui->actionClose_Tab->setEnabled(enabled);

    this->updateContext();
}

void MainWindow::updateContext() {
    ZzzRequestTreeItemPtr request = this->currentRequest();

    if (request) {
        if (request.dynamicCast<ZzzRequest>()) {
            ui->actionSend->setEnabled(true);
        } else {
            ui->actionSend->setEnabled(false);
        }
    } else {
        ui->actionSend->setEnabled(false);
    }

#ifdef Q_OS_MAC
    this->updateContextMacOs();
#endif
}

ZzzRequestTreeItemPtr MainWindow::currentRequest() {
    ZzzRequestTreeItemPtr request;
    auto browser = qobject_cast<ZzzWorkspaceEditor*>(ui->stackedWidget->currentWidget());
    if (browser) {
        return browser->currentRequest();
    }
    return {};
}

void MainWindow::on_stackedWidget_currentChanged(int arg1) {
    this->updateContext();
}

void MainWindow::on_actionSend_triggered() {
    this->executeCurrentRequest();
}

void MainWindow::executeCurrentRequest() {
    auto browser = qobject_cast<ZzzWorkspaceEditor*>(ui->stackedWidget->currentWidget());
    if (browser) {
        browser->executeCurrentRequest();
    }
}

void MainWindow::on_actionResponses_toggled(bool arg1) {
    ui->replyViewer->setVisible(arg1);
    ui->line->setVisible(arg1);
}
