#include "document.hpp"

namespace Data
{
document::document()
    : category{categories::None}
    , uploadDate{}
{}

QHash<int, QByteArray> document::roleNames()
{
    QHash<int, QByteArray> names;

    names[CategoryRole] = "category";
    names[LocalPathRole] = "localPath";
    names[FileNameRole] = "fileName";
    names[ExtensionRole] = "extension";
    names[StateRole] = "state";
    names[UploadProgressRole] = "uploadProgress";
    names[UploadDateRole] = "uploadDate";
    names[IdRole] = "id";

    return names;
}

QVariant document::data(int role) const
{
    switch (role)
    {
    case CategoryRole:
        return category;
    case RelativePathRole:
        return relativePath;
    case LocalPathRole:
        return localPath;
    case FileNameRole:
        return fileName;
    case ExtensionRole:
        return extension;
    case StateRole:
        return state;
    case UploadProgressRole:
        return uploadProgress;
    case UploadDateRole:
        return uploadDate;
    case IdRole:
        return id;
    }

    return QVariant{};
}

void document::setData(const QVariant &value, int role)
{
    switch (role)
    {
    case CategoryRole:
        category = categories(value.toInt());
        break;
    case LocalPathRole:
    {
        localPath = value.toUrl();
        set_file_info();
        state = NotUploded;
        break;
    }
    }
}

void document::read(const QJsonObject& json)
{
    if (json.contains("category") && json["category"].isDouble())
        category = categories(json["category"].toInt());

    if (json.contains("isUploaded") && json["isUploaded"].isBool())
        json["isUploaded"].toBool() ? state = Uploaded : state = NotUploded;

    if(state == Uploaded)
    {
        if (json.contains("uploadDate") && json["uplaodDate"].isString())
            uploadDate = QDate::fromString(json["uploadDate"].toString(), "jj.MM.yyyy");
    }

    bool name_and_extension{false};

    if (json.contains("fileName") && json["fileName"].isString())
    {
        fileName = json["fileName"].toString();
        name_and_extension = true;
    }

    if (json.contains("extension") && json["extension"].isString())
    {
        extension = json["extension"].toString();
        name_and_extension = name_and_extension && true;
    }

    if (json.contains("localPath") && json["localPath"].isString())
        QUrl url{json["relativePath"].toString()};

    if (json.contains("relativePath") && json["relativePath"].isString())
        relativePath = json["relativePath"].toString();

    if (!name_and_extension)
        set_file_info();

    if (json.contains("id") && json["id"].isDouble())
        id = json["id"].toInt();
}

void document::write(QJsonObject& json) const
{
    json["category"] = category;
    json["isUploaded"] = state == Uploaded;
    json["fileName"] = fileName;
    json["extension"] = extension;
    json["id"] = id;
}

bool document::is_completed() const
{
    if (fileName.isEmpty())
        return false;

    return state == Uploaded;
}

void document::set_file_info()
{
    if (localPath.isEmpty() && relativePath.isEmpty())
        return;

    QString fullPath{};

    if (!localPath.isEmpty())
        fullPath = localPath.toLocalFile();
    else
        fullPath = relativePath.toLocalFile();

    auto withExtension{fullPath.split(QLatin1Char('/')).back()};
    auto splitedFile{withExtension.split(QLatin1Char('.'))};

    extension = splitedFile.back();
    splitedFile.pop_back();
    fileName = splitedFile.join(QLatin1Char('.'));
}
}
