#include "environmentprovidereditor.h"
#include "ui_environmentprovidereditor.h"

#include "providers/environmentprovider.h"
#include "workspacefile.h"
#include <QCoroTask>
#include <QMenu>
#include <ranges/trange.h>
#include <tinputdialog.h>
#include <tmessagebox.h>

struct EnvironmentProviderEditorPrivate {
        EnvironmentProvider* environmentProvider;
};

EnvironmentProviderEditor::EnvironmentProviderEditor(EnvironmentProvider* environmentProvider, QWidget* parent) :
    ProviderEditor(environmentProvider->workspace()->sharedFromThis().staticCast<WorkspaceFile>()),
    ui(new Ui::EnvironmentProviderEditor) {
    ui->setupUi(this);
    d = new EnvironmentProviderEditorPrivate();
    d->environmentProvider = environmentProvider;
}

EnvironmentProviderEditor::~EnvironmentProviderEditor() {
    delete ui;
    delete d;
}

QString EnvironmentProviderEditor::text() {
    return tr("Env", "Short for Environment");
}

int EnvironmentProviderEditor::order() {
    return 30;
}

void EnvironmentProviderEditor::updateData() {
    QSignalBlocker blocker(ui->tableWidget);
    ui->tableWidget->clear();

    ui->tableWidget->setColumnCount(d->environmentProvider->environments().length() + 2);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Variable")));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Secret?")));
    for (auto i = 0; i < d->environmentProvider->environments().length(); i++) {
        auto [uuid, name] = d->environmentProvider->environments().at(i);
        auto item = new QTableWidgetItem();
        item->setText(name);
        item->setData(Qt::UserRole, uuid);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
        ui->tableWidget->setHorizontalHeaderItem(2 + i, item);
    }

    ui->tableWidget->setRowCount(d->environmentProvider->variables().length() + 1);
    for (auto i = 0; i < d->environmentProvider->variables().length(); i++) {
        auto var = d->environmentProvider->variables().at(i);
        auto [varUuid, name, isSecret] = var;

        auto item = new QTableWidgetItem();
        item->setText(name);
        item->setData(Qt::UserRole, varUuid);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(i, 0, item);

        auto secretItem = new QTableWidgetItem();
        secretItem->setCheckState(isSecret ? Qt::Checked : Qt::Unchecked);
        secretItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        ui->tableWidget->setItem(i, 1, secretItem);

        for (auto j = 0; j < d->environmentProvider->environments().length(); j++) {
            auto env = d->environmentProvider->environments().at(j);
            auto envVar = tRange(d->environmentProvider->environmentVariables()).first([env, var](ZzzEnvironmentVariable envVar) {
                auto [varUuid, name, isSecret] = var;
                auto [envUuid, envName] = env;
                return std::get<0>(envVar) == envUuid && std::get<1>(envVar) == varUuid;
            },
                {});

            auto envVarItem = new QTableWidgetItem();
            envVarItem->setText(std::get<2>(envVar));
            envVarItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
            ui->tableWidget->setItem(i, 2 + j, envVarItem);
        }
    }

    auto addItem = new QTableWidgetItem();
    addItem->setText(tr("New Variable..."));
    addItem->setData(Qt::UserRole, QUuid::createUuid());
    addItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    ui->tableWidget->setItem(d->environmentProvider->variables().length(), 0, addItem);
    for (auto i = 1; i < ui->tableWidget->columnCount(); i++) {
        auto disabledItem = new QTableWidgetItem();
        disabledItem->setFlags(Qt::NoItemFlags);
        ui->tableWidget->setItem(d->environmentProvider->variables().length(), i, disabledItem);
    }
}

void EnvironmentProviderEditor::on_tableWidget_itemChanged(QTableWidgetItem* item) {
    QList<ZzzVariable> variables;
    QList<ZzzEnvironmentVariable> environmentVariables;

    // Collect all the variables
    for (auto i = 0; i < ui->tableWidget->rowCount(); i++) {
        auto name = ui->tableWidget->item(i, 0);
        auto secret = ui->tableWidget->item(i, 1);

        if (name->text().isEmpty() || name->text() == tr("New Variable...")) continue;

        variables.append({name->data(Qt::UserRole).toUuid(), name->text(), secret ? secret->checkState() == Qt::Checked : false});
    }

    // Collect all the environment variables
    for (auto i = 0; i < ui->tableWidget->rowCount(); i++) {
        auto variableUuid = ui->tableWidget->item(i, 0)->data(Qt::UserRole).toUuid();
        for (auto j = 0; j < d->environmentProvider->environments().count(); j++) {
            auto [environmentUuid, environmentName] = d->environmentProvider->environments().at(j);
            auto cell = ui->tableWidget->item(i, j + 2);
            environmentVariables.append({environmentUuid, variableUuid, cell ? cell->text() : ""});
        }
    }

    d->environmentProvider->setVariables(variables);
    d->environmentProvider->setEnvironmentVariables(environmentVariables);
}

void EnvironmentProviderEditor::on_tableWidget_customContextMenuRequested(const QPoint& pos) {
    auto item = ui->tableWidget->itemAt(pos);
    if (item) {
        auto menu = new QMenu(this);

        if (item->column() > 1) {
            auto env = d->environmentProvider->environments().at(item->column() - 2);
            auto [envUuid, envName] = env;
            menu->addSection(tr("For Environment %1").arg(QLocale().quoteString(envName)));
            menu->addAction(QIcon::fromTheme("edit-delete"), tr("Delete Environment"), [this, env]() -> QCoro::Task<> {
                auto innerEnv = env;
                auto [envUuid, envName] = innerEnv;

                if (d->environmentProvider->environments().length() <= 1) {
                    tMessageBox box(this->window());
                    box.setTitleBarText(tr("Unable to delete environment"));
                    box.setMessageText(tr("Every workspace must have at least one environment."));
                    box.setInformativeText(tr("To delete the %1 environment, create a new environment first.").arg(QLocale().quoteString(envName)));
                    box.setIcon(QMessageBox::Warning);
                    auto button = co_await box.presentAsync();
                    co_return;
                }

                tMessageBox box(this->window());
                box.setTitleBarText(tr("Delete Environment"));
                box.setMessageText(tr("Do you want to delete the environment %1?").arg(QLocale().quoteString(envName)));
                box.setInformativeText(tr("Deleting the environment will also delete all environment variables associated with it."));
                box.setIcon(QMessageBox::Warning);
                auto deleteButton = box.addButton(tr("Delete Environment"), QMessageBox::DestructiveRole);
                auto cancelButton = box.addStandardButton(QMessageBox::Cancel);

                auto button = co_await box.presentAsync();
                if (button == cancelButton) co_return;

                d->environmentProvider->removeEnvironment(envUuid);
            });
        }

        connect(menu, &QMenu::aboutToHide, menu, &QMenu::deleteLater);
        menu->popup(ui->tableWidget->mapToGlobal(pos));
    }
}

void EnvironmentProviderEditor::on_newEnvironmentButton_clicked() {
    bool ok;
    auto environmentName = tInputDialog::getText(this->window(), tr("New Environment"), tr("What do you want to call the new environment?"), QLineEdit::Normal, "", &ok);
    if (ok) {
        d->environmentProvider->addEnvironment(environmentName);
    }
}
