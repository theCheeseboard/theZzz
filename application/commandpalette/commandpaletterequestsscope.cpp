#include "commandpaletterequestsscope.h"

#include <widgets/zzzworkspaceeditor.h>
#include <workspacefile.h>
#include <zzzhelpers.h>
#include <zzzrequest.h>
#include <zzzrequesttreeitem.h>

struct CommandPaletteRequestsScopePrivate {
        ZzzWorkspaceEditor* workspaceEditor;
        QString query;
        QList<ZzzRequestTreeItemPtr> requests;
};

CommandPaletteRequestsScope::CommandPaletteRequestsScope(ZzzWorkspaceEditor* workspaceEditor, QObject* parent) :
    tCommandPaletteScope{parent} {
    d = new CommandPaletteRequestsScopePrivate();
    d->workspaceEditor = workspaceEditor;

    connect(d->workspaceEditor->workspace().data(), &WorkspaceFile::requestsChanged, this, &CommandPaletteRequestsScope::updateData);
    this->updateData();
}

CommandPaletteRequestsScope::~CommandPaletteRequestsScope() {
    delete d;
}

void CommandPaletteRequestsScope::updateData() {
    d->requests.clear();
    appendMatchingRequests(d->workspaceEditor->workspace());
}

void CommandPaletteRequestsScope::appendMatchingRequests(RequestContainerProviderPtr provider) {
    for (auto request : provider->requests()) {
        if (d->query.isEmpty() || request->treeWidgetItem()->text(0).contains(d->query, Qt::CaseInsensitive)) {
            d->requests.append(request);
        }

        if (auto containerProvider = request.dynamicCast<RequestContainerProvider>()) {
            appendMatchingRequests(containerProvider);
        }
    }
}

int CommandPaletteRequestsScope::rowCount(const QModelIndex& parent) const {
    return d->requests.count();
}

QVariant CommandPaletteRequestsScope::data(const QModelIndex& index, int role) const {
    auto abstractRequest = d->requests.at(index.row());

    if (role == Qt::DisplayRole) {
        return abstractRequest->treeWidgetItem()->text(0);
    } else if (role == Qt::DecorationRole) {
        if (auto request = abstractRequest.dynamicCast<ZzzRequest>()) {
            return ZzzHelpers::iconForVerb(request->verb());
        }
    }

    return {};
}

QString CommandPaletteRequestsScope::displayName() {
    return tr("Requests");
}

void CommandPaletteRequestsScope::filter(QString filter) {
    d->query = filter;
    this->updateData();
}

void CommandPaletteRequestsScope::activate(QModelIndex index) {
    auto request = d->requests.at(index.row());
    auto treeWidget = request->treeWidgetItem()->treeWidget();
    treeWidget->clearSelection();
    request->treeWidgetItem()->setSelected(true);
    auto parent = request->treeWidgetItem();
    while ((parent = parent->parent())) {
        parent->setExpanded(true);
    }

    emit showWorkspace();
}
