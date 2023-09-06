#ifndef ZZZWORKSPACEEDITOR_H
#define ZZZWORKSPACEEDITOR_H

#include "forwarddeclares.h"
#include <QStyledItemDelegate>
#include <QWidget>
#include <tpaintcalculator.h>

namespace Ui {
    class ZzzWorkspaceEditor;
}

class QTreeWidgetItem;
struct ZzzWorkspaceEditorPrivate;
class ZzzWorkspaceEditor : public QWidget {
        Q_OBJECT

    public:
        explicit ZzzWorkspaceEditor(QWidget* parent = nullptr);
        ~ZzzWorkspaceEditor();

        QString currentFile();
        void saveWorkspace(QString filePath);
        void openWorkspace(QString filePath);

    private slots:
        void on_newRequestButton_clicked();

        void on_treeWidget_itemSelectionChanged();

    signals:
        void addReply(ZzzReplyPtr reply);

    private:
        Ui::ZzzWorkspaceEditor* ui;
        ZzzWorkspaceEditorPrivate* d;

        void updateRequests(QTreeWidgetItem* rootItem, RequestContainerProviderPtr containerProvider);
};

class ZzzWorkspaceEditorRequestDelegate : public QStyledItemDelegate {
        Q_OBJECT
    public:
        ZzzWorkspaceEditorRequestDelegate(QObject* parent = nullptr);

    private:
        tPaintCalculator paintCalculator(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

        // QAbstractItemDelegate interface
    public:
        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
        QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // ZZZWORKSPACEEDITOR_H
