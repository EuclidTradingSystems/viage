#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include "qurl.h"
#include "qvariant.h"
#include "qjsonobject.h"

#include "base_item.hpp"

namespace Data
{
struct document final : public base_item<document>
{
    document();

    static consteval auto key() { return "document"; };
    static consteval auto qmlName() { return "Document"; };
    static consteval auto uri() { return "Data"; };

    enum categories
    {
        None = 0,
        Picture = 1,
        Passeport = 2,
        RegisteryExcerpt = 4,
        PursuitExcerpt = 8,
        TaxDeclaration = 16,
        BuildingDetails = 32,
        Insurance = 64,
        Discernement = 128,
        Beb = 256,
        Jobs = 512,
        FutureJobs = 1024,
        Calculation = 2048,
        Other = 4096,
        Maintenance = 8192,
        Constitution = 16384,
        PPE = 32768,
        PPEPVS = 65536
    };

    enum states
    {
        NotUploded,
        Uploading,
        Uploaded
    };

    template <typename T>
    static bool documents_completed(const QVector<T>& docs, bool ppe)
    {
        bool uploaded{true};
        int flags{categories::None};

        for (const auto& doc : docs)
        {
            if (doc.state != states::Uploaded)
            {
                uploaded = false;
                break;
            }

            flags |= doc.category;
        }

        if (uploaded)
        {
            bool res{flags & Picture
                     && flags & Passeport
                     && flags & RegisteryExcerpt
                     && flags & PursuitExcerpt
                     && flags & TaxDeclaration
                     && flags & BuildingDetails
                     && flags & Insurance
                     && flags & Discernement
                     && flags & Calculation};

            if (!res) return false;

            if (ppe)
                res = flags & Constitution
                      && flags & PPE
                      && flags & PPEPVS;

            return res;
        }
        else
            return false;
    }

    enum roles
    {
        CategoryRole = Qt::UserRole,
        RelativePathRole,
        LocalPathRole,
        FileNameRole,
        ExtensionRole,
        StateRole,
        UploadProgressRole,
        UploadDateRole,
        IdRole
    };

    static QHash<int, QByteArray> roleNames();

    QVariant data(int role) const;
    void setData(const QVariant& value, int role);

    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) const override;

    bool is_completed() const override;

    int id;
    QUrl localPath;
    categories category;
    states state;

private:
    QUrl relativePath;
    QString fileName;
    QString extension;
    float uploadProgress;
    QDate uploadDate;

    void set_file_info();
};

}

#endif // DOCUMENT_HPP
