#ifndef COMMANDPALETTEREQUESTSSCOPE_H
#define COMMANDPALETTEREQUESTSSCOPE_H

#include <providers/requestcontainerprovider.h>
#include <tcommandpalette/tcommandpalettescope.h>

class ZzzWorkspaceEditor;
struct CommandPaletteRequestsScopePrivate;
class CommandPaletteRequestsScope : public tCommandPaletteScope {
        Q_OBJECT
    public:
        explicit CommandPaletteRequestsScope(ZzzWorkspaceEditor* workspaceEditor, QObject* parent = nullptr);
        ~CommandPaletteRequestsScope();

    signals:
        void showWorkspace();

    private:
        CommandPaletteRequestsScopePrivate* d;

        void updateData();
        void appendMatchingRequests(RequestContainerProviderPtr provider);

        // QAbstractItemModel interface
    public:
        int rowCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;

        // tCommandPaletteScope interface
    public:
        QString displayName();
        void filter(QString filter);
        void activate(QModelIndex index);
};

#endif // COMMANDPALETTEREQUESTSSCOPE_H
