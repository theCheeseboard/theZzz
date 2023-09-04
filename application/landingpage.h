#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QWidget>

namespace Ui {
    class LandingPage;
}

class LandingPage : public QWidget {
        Q_OBJECT

    public:
        explicit LandingPage(QWidget* parent = nullptr);
        ~LandingPage();

    signals:
        void newWorkspace();
        void openWorkspace();

    private slots:
        void on_newWorkspaceButton_clicked();

        void on_openWorkspaceButton_clicked();

    private:
        Ui::LandingPage* ui;
};

#endif // LANDINGPAGE_H
