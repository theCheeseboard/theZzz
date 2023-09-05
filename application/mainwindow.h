#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoroTask>
#include <QMainWindow>

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

    private:
        Ui::MainWindow* ui;
        MainWindowPrivate* d;

        ZzzWorkspaceEditor* newTab();

        void updateMenuItems();
};

#endif // MAINWINDOW_H
