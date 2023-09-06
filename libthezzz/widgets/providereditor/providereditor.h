#ifndef PROVIDEREDITOR_H
#define PROVIDEREDITOR_H

#include "forwarddeclares.h"
#include <QWidget>

struct ProviderEditorPrivate;
class ProviderEditor : public QWidget {
        Q_OBJECT
    public:
        explicit ProviderEditor(WorkspaceFilePtr workspace, QWidget* parent = nullptr);
        ~ProviderEditor();

        virtual QString text() = 0;
        virtual int order() = 0;

    signals:

    protected:
        virtual void updateData() = 0;

    private:
        ProviderEditorPrivate* d;
};

#endif // PROVIDEREDITOR_H
