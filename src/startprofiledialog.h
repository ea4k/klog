#ifndef KLOGNG_STARTPROFILEDIALOG_H
#define KLOGNG_STARTPROFILEDIALOG_H

#include <QDialog>
#include "profilemanager.h"

class QTableWidget;
class World;
class DataProxy_SQLite;
class QCheckBox;
class QPushButton;

class StartProfileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StartProfileDialog(ProfileManager *pm, World *world = nullptr, DataProxy_SQLite *dp = nullptr, QWidget *parent = nullptr);

    int  selectedProfileId() const { return selectedId; }

    static int chooseProfileOnStartup(ProfileManager *pm, World *world = nullptr, DataProxy_SQLite *dp = nullptr, QWidget *parent = nullptr);

private slots:
    void openSelected();
    void newProfile();
    void editSelected();
    void deleteSelected();

private:
    void reload();
    int  currentRowProfileId() const;

    ProfileManager *pm;
    World          *world = nullptr;
    DataProxy_SQLite *dataProxy = nullptr;
    QTableWidget   *table;
    QCheckBox      *openLastCheck;
    QPushButton    *openBtn;
    QPushButton    *editBtn;
    QPushButton    *deleteBtn;
    int             selectedId = -1;
};

#endif
