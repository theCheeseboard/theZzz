#ifndef ZZZREQUESTTREEWIDGET_H
#define ZZZREQUESTTREEWIDGET_H

#include <QTreeWidget>
#include <QWidget>

struct ZzzRequestTreeWidgetPrivate;
class ZzzRequestTreeWidget : public QTreeWidget {
        Q_OBJECT
    public:
        explicit ZzzRequestTreeWidget(QWidget* parent);
        ~ZzzRequestTreeWidget();

    private:
        ZzzRequestTreeWidgetPrivate* d;

        // QWidget interface
    protected:
        void dragEnterEvent(QDragEnterEvent* event);
        void dropEvent(QDropEvent* event);
};

#endif // ZZZREQUESTTREEWIDGET_H
