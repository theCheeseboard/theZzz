#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoroTask>
#include <QMainWindow>
#include <repository.h>

namespace Ui {
    class MainWindow;
}

class ZzzWorkspaceEditor;
class RepositoryBrowser;
struct MainWindowPrivate;
class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

        void openRepo(QString path);

        RepositoryBrowser* openNextTab();

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

    private:
        Ui::MainWindow* ui;
        MainWindowPrivate* d;

        ZzzWorkspaceEditor* newTab();

        void updateMenuItems();

        QCoro::Task<RepositoryPtr> currentFileGitRepository();
};

#endif // MAINWINDOW_H
