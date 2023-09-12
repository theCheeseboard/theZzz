#include "bodyprovidereditor.h"
#include "ui_bodyprovidereditor.h"

#include "providers/bodyprovider.h"
#include "workspacefile.h"
#include <QActionGroup>
#include <QMenu>
#include <texteditor.h>
#include <tinputdialog.h>

struct BodyProviderEditorPrivate {
        BodyProvider* provider;
        QActionGroup* actionGroup;
        QList<std::pair<QAction*, QString>> contentTypeMapping;
};

BodyProviderEditor::BodyProviderEditor(BodyProvider* bodyProvider, QWidget* parent) :
    ProviderEditor(bodyProvider->workspace()->sharedFromThis().staticCast<WorkspaceFile>(), parent),
    ui(new Ui::BodyProviderEditor) {
    ui->setupUi(this);

    d = new BodyProviderEditorPrivate();
    d->provider = bodyProvider;

    connect(ui->editor->editor(), &TextEditor::textChanged, this, [this] {
        d->provider->setBody(ui->editor->editor()->text().toUtf8());
    });

    d->contentTypeMapping.append({ui->actionPlain_Text, QStringLiteral("text/plain")});
    d->contentTypeMapping.append({ui->actionJSON, QStringLiteral("application/json")});
    d->contentTypeMapping.append({ui->actionForm_Data, QStringLiteral("multipart/form-data")});
    d->contentTypeMapping.append({ui->actionx_www_form_urlencoded, QStringLiteral("application/x-www-form-urlencoded")});
    d->contentTypeMapping.append({ui->actionXML, QStringLiteral("application/xml")});
    d->contentTypeMapping.append({ui->actionYAML, QStringLiteral("text/yaml")});
    d->contentTypeMapping.append({ui->actionCustom, QStringLiteral("")});

    QMenu* contentTypeMenu = new QMenu(this);
    contentTypeMenu->addSection(tr("Content Type"));
    d->actionGroup = new QActionGroup(this);
    d->actionGroup->setExclusive(true);
    for (const auto& action : d->contentTypeMapping) {
        d->actionGroup->addAction(action.first);
        contentTypeMenu->addAction(action.first);
    }
    ui->contentTypeButton->setMenu(contentTypeMenu);

    connect(d->actionGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        for (auto a : d->contentTypeMapping) {
            if (a.first == action && !a.second.isEmpty() && a.second != QStringLiteral("multipart/form-data")) {
                d->provider->setContentType(a.second);
                return;
            }
        }
    });
}

BodyProviderEditor::~BodyProviderEditor() {
    delete ui;
    delete d;
}

QString BodyProviderEditor::text() {
    return tr("Body", "Short for Body");
}

int BodyProviderEditor::order() {
    return 200;
}

void BodyProviderEditor::updateData() {
    if (!ui->editor->editor()->hasFocus()) {
        QSignalBlocker blocker(ui->editor->editor());
        ui->editor->editor()->setText(d->provider->body());
    }

    auto contentType = d->provider->contentType().split(";").first().trimmed();

    for (const auto& a : d->contentTypeMapping) {
        if (a.second == contentType) {
            ui->contentTypeButton->setText(a.first->text());
            a.first->setChecked(true);
            return;
        }
    }

    ui->contentTypeButton->setText(contentType);
    ui->actionCustom->setChecked(true);
}

void BodyProviderEditor::on_actionCustom_triggered() {
    bool ok;
    auto contentType = tInputDialog::getText(this->window(), tr("Custom Content Type"), tr("What content type are you sending?"), QLineEdit::Normal, d->provider->contentType(), &ok);
    if (ok) {
        d->provider->setContentType(contentType);
    }

    this->updateData();
}

void BodyProviderEditor::on_actionForm_Data_triggered() {
    auto boundary = QStringLiteral("--XXX-BOUNDARY-%1-XXX-").arg(QUuid::createUuid().toString(QUuid::WithoutBraces));
    d->provider->setContentType(QStringLiteral("multipart/form-data; boundary=%1").arg(boundary));
}
