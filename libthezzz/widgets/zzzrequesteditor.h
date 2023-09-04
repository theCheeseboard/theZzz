#ifndef ZZZREQUESTEDITOR_H
#define ZZZREQUESTEDITOR_H

#include "forwarddeclares.h"
#include <QWidget>

namespace Ui {
    class ZzzRequestEditor;
}

struct ZzzRequestEditorPrivate;
class ZzzRequestEditor : public QWidget {
        Q_OBJECT

    public:
        explicit ZzzRequestEditor(ZzzRequestPtr request, QWidget* parent = nullptr);
        ~ZzzRequestEditor();

    private:
        Ui::ZzzRequestEditor* ui;
        ZzzRequestEditorPrivate* d;
};

#endif // ZZZREQUESTEDITOR_H
