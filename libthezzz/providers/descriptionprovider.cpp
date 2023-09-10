#include "descriptionprovider.h"
#include "widgets/providereditor/descriptionprovidereditor.h"

#include "workspacefile.h"

struct DescriptionProviderPrivate {
        QString title;
        QString description;
};

DescriptionProvider::DescriptionProvider(WorkspaceFile* parent) :
    ZzzProvider(parent) {
    d = new DescriptionProviderPrivate();
}

DescriptionProvider::~DescriptionProvider() {
    delete d;
}

QString DescriptionProvider::title() {
    return d->title;
}

void DescriptionProvider::setTitle(QString title) {
    d->title = title;
    emit this->workspace()->dataChanged();
}

QString DescriptionProvider::description() {
    return d->description;
}

void DescriptionProvider::setDescription(QString description) {
    d->description = description;
    emit this->workspace()->dataChanged();
}

QString DescriptionProvider::jsonKey() {
    return QStringLiteral("description");
}

void DescriptionProvider::loadJson(QJsonValue obj, QJsonValue localObj) {
    auto object = obj.toObject();
    d->title = object.value("title").toString();
    d->description = object.value("description").toString();
}

QJsonValue DescriptionProvider::toJson() {
    return QJsonObject{
        {"title",       d->title      },
        {"description", d->description}
    };
}

QList<ProviderEditor*> DescriptionProvider::editor() {
    return {new DescriptionProviderEditor(this)};
}

QJsonValue DescriptionProvider::toLocalJson() {
    return QJsonObject{
        {"title", d->title}
    };
}
