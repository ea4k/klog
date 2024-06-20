#include "db_adif_primary_subdvisions_data.h"

DB_ADIF_Primary_Subdvisions_data::DB_ADIF_Primary_Subdvisions_data(const QString &_parentClass)
{
    Q_UNUSED(_parentClass);
    qDebug() << Q_FUNC_INFO << _parentClass ;
}

bool DB_ADIF_Primary_Subdvisions_data::addData()
{

/*

    stringQuery = stringQuery + QString(" (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "dxcc INTEGER NOT NULL, "       // arrlId (281)
        "name VARCHAR NOT NULL, "       // Madrid
        "shortname VARCHAR NOT NULL, "  // M
        "prefix VARCHAR, "              // EA4
        "cqz INTEGER NOT NULL, "        // 14
        "ituz INTEGER NOT NULL, "       // 37
        "regionalgroup VARCHAR, "       // Comunidad de Madrid
        "regionalid INTEGER, "          // Oblast in Russia or any other Id
        "start_date DATETIME, "
        "end_date DATETIME, "
        "deleted VARCHAR, "
        "UNIQUE (id, shortname, name), "
        "FOREIGN KEY (cqz) REFERENCES entity (cqz), "
        "FOREIGN KEY (ituz) REFERENCES entity (ituz), "
        "FOREIGN KEY (dxcc) REFERENCES entity (dxcc) )");
*/

    if (!isDBCreated())
        return false;
    if (!add_EA6_21())
        return false;
    return add_EA_281();
}

bool DB_ADIF_Primary_Subdvisions_data::isDBCreated()
{
    return true;
}

bool DB_ADIF_Primary_Subdvisions_data::addValues(const QString &rows, const QStringList &values)
{
    // This function will add all the values that come in the QStringList
    // It receives the list of rows and the values that will feed the table with the
    // values for primary subdivisions (provinces?) of the DXCC entity.
    QueryExecutor exe(Q_FUNC_INFO);
    QString sq = QString("INSERT INTO primary_subdivisions (%1) VALUES ").arg(rows);
    QString aux, oneValue;

    foreach (aux, values)
    {
        oneValue = QString("%1 (%2))").arg(sq).arg(aux);
        if (!exe.execQuery(Q_FUNC_INFO, oneValue))
            return false;
    }

    return true;
}

bool DB_ADIF_Primary_Subdvisions_data::add_EA6_21()
{
    // This function contains the Primary Subdivision data for Balearic is
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values << "'21', 'Baleares', 'IB', 'EA6', '14', '37', 'Islas Baleares', '-1', '', '', 'N'";

    return addValues(rows, values);
}


bool DB_ADIF_Primary_Subdvisions_data::add_EA_281()
{
    // This function contains the Primary Subdivision data for Spain
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values << "'281', 'Madrid', 'M', 'EA4', '14', '37', 'Madrid', '-1', '', '', 'N'"
        << "'281', 'Asturias', 'O', 'EA1', '14', '37', 'Asturias', '-1', '', '', 'N'"
        << "'281', 'Avila', 'AV', 'EA1', '14', '37', 'Castilla y León', '-1', '', '', 'N'";

    return addValues(rows, values);
}

