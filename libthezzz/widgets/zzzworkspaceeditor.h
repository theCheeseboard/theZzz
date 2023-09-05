#ifndef ZZZWORKSPACEEDITOR_H
#define ZZZWORKSPACEEDITOR_H

#include "forwarddeclares.h"
#include <QWidget>

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

#endif // ZZZWORKSPACEEDITOR_H
