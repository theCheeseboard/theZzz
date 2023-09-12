#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "forwarddeclares.h"
#include <QCoroTask>
#include <QMainWindow>
#include <objects/repository.h>

namespace Ui {
    class MainWindow;
}

class ZzzWorkspaceEditor;
class RepositoryBrowser;
struct MainWindowPrivate;
struct MainWindowMacPrivate;
class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

        void openRepo(QString path);

        RepositoryBrowser* openNextTab();

        void executeCurrentRequest();
        ZzzRequestTreeItemPtr currentRequest();

    private slots:
        void on_actionExit_triggered();

        void on_actionNew_Workspace_triggered();

        void on_actionSave_triggered();

        QCoro::Task<> on_actionSave_As_triggered();

        QCoro::Task<> on_actionOpen_Workspace_triggered();

        QCoro::Task<> on_actionCommit_triggered();

        QCoro::Task<> on_actionPush_triggered();

        QCoro::Task<> on_actionPull_triggered();

        QCoro::Task<> on_actionCheckout_triggered();

        void on_actionClose_Tab_triggered();

        void on_stackedWidget_currentChanged(int arg1);

        void on_actionSend_triggered();

        void on_actionResponses_toggled(bool arg1);

    private:
        Ui::MainWindow* ui;
        MainWindowPrivate* d;

        ZzzWorkspaceEditor* newTab();

        void updateMenuItems();
        void updateContext();

#ifdef Q_OS_MAC
        MainWindowMacPrivate* md;

        void setupMacOs();
        void updateContextMacOs();
        void destroyMacOs();
#endif

        QCoro::Task<RepositoryPtr> currentFileGitRepository();
};

#endif // MAINWINDOW_H
