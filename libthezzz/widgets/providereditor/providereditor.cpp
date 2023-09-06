#include "providereditor.h"

#include "workspacefile.h"
#include <QTimer>

struct ProviderEditorPrivate {
        WorkspaceFilePtr workspace;
};

ProviderEditor::ProviderEditor(WorkspaceFilePtr workspace, QWidget* parent) :
    QWidget{parent} {
    d = new ProviderEditorPrivate();
    d->workspace = workspace;

    connect(workspace.data(), &WorkspaceFile::dataChanged, this, &ProviderEditor::updateData);
    QTimer::singleShot(0, this, &ProviderEditor::updateData);
}

ProviderEditor::~ProviderEditor() {
    delete d;
}
