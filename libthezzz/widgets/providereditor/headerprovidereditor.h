#ifndef HEADERPROVIDEREDITOR_H
#define HEADERPROVIDEREDITOR_H

#include "providereditor.h"
#include "zzzhelpers.h"

namespace Ui {
    class HeaderProviderEditor;
}

class HeadersProvider;
class QTreeWidgetItem;
struct HeaderProviderEditorPrivate;
class HeaderProviderEditor : public ProviderEditor {
        Q_OBJECT

    public:
        explicit HeaderProviderEditor(HeadersProvider* headersProvider, QWidget* parent = nullptr);
        ~HeaderProviderEditor();

    private:
        Ui::HeaderProviderEditor* ui;
        HeaderProviderEditorPrivate* d;

        // ProviderEditor interface
    public:
        QString text();
        int order();

    protected:
        void updateData();
    private slots:
        void on_headersList_itemChanged(QTreeWidgetItem* item, int column);
};

#endif // HEADERPROVIDEREDITOR_H
