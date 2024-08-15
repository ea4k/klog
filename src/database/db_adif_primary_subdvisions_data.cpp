#include "db_adif_primary_subdvisions_data.h"

DB_ADIF_Primary_Subdvisions_data::DB_ADIF_Primary_Subdvisions_data(const QString &_parentClass)
{
    Q_UNUSED(_parentClass);
    //qDebug() << Q_FUNC_INFO << _parentClass ;
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

    qDebug() << Q_FUNC_INFO <<  " - Checking if DB exists";
    if (!isDBCreated())
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 1";
    if (!add_Canada_1())       // Adds the data for Canada
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 6";
    if (!add_USA_6())       // Adds the data for Alaska
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 21";
    if (!add_EA6_21())      // Adds the data for Balearic Is
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 29";
    if (!add_EA8_29())      // Adds the data for Canary Is
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 32";
    if (!add_EA9_32())      // Adds the data for Ceuta y Melilla
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 50";
    if (!add_Mexico_50())      // Adds the data for Mexico
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 100";
    if (!add_Argentina_100())      // Adds the data for Argentina
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 108";
    if (!add_Brazil_108())      // Adds the data for Brazil
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 110";
    if (!add_USA_110())     // Adds the data for Hawaii
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 149";
    if (!add_CT_149())     // Adds the data for Azores
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 256";
    if (!add_CT_256())      // Adds the data for Madeira
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 272";
    if (!add_CT_272())      // Adds the data for Portugal
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 281";
    if (!add_EA_281())     // Adds the data for Spain
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 291";
    if (!add_USA_291())     // Adds the data for United States of America
        return false;
    qDebug() << Q_FUNC_INFO <<  " - Adding 339";
    if (!add_Japan_339())     // Adds the data for Japan
        return false;

    return true;
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
        oneValue = QString("%1 (%2)").arg(sq, aux);
        if (!exe.execQuery(Q_FUNC_INFO, oneValue))
            return false;
    }

    return true;
}

bool DB_ADIF_Primary_Subdvisions_data::add_Canada_1()
{
    // This function contains the Primary Subdivision data for Canada
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values
        << "'1', 'Nova Scotia', 'NS', 'VE1', '05', '09', '', '-1', '', '', 'N'"
        << "'1', 'Quebec', 'QC', 'VE2', '02', '04', '', '-1', '', '', 'N'"
        << "'1', 'Quebec', 'QC', 'VE2', '05', '09', '', '-1', '', '', 'N'"
        << "'1', 'Ontario', 'ON', 'VE3', '04', '03', '', '-1', '', '', 'N'"
        << "'1', 'Ontario', 'ON', 'VE3', '04', '04', '', '-1', '', '', 'N'"
        << "'1', 'Manitoba', 'MB', 'VE4', '04', '03', '', '-1', '', '', 'N'"
        << "'1', 'Manitoba', 'MB', 'VE4', '04', '04', '', '-1', '', '', 'N'"
        << "'1', 'Saskatchewan', 'SK', 'VE5', '04', '03', '', '-1', '', '', 'N'"
        << "'1', 'Alberta', 'AB', 'VE6', '04', '02', '', '-1', '', '', 'N'"
        << "'1', 'British Columbia', 'BC', 'VE7', '03', '02', '', '-1', '', '', 'N'"
        << "'1', 'Northwest Territories', 'NT', 'VE8', '01', '03', '', '-1', '', '', 'N'"
        << "'1', 'Northwest Territories', 'NT', 'VE8', '02', '04', '', '-1', '', '', 'N'"
        << "'1', 'Northwest Territories', 'NT', 'VE8', '04', '75', '', '-1', '', '', 'N'"
        << "'1', 'New Brunswick', 'NB', 'VE9', '05', '09', '', '-1', '', '', 'N'"
        << "'1', 'New foundland', 'NL', 'VO1', '02', '09', '', '-1', '', '', 'N'"
        << "'1', 'New foundland', 'NL', 'VO1', '05', '09', '', '-1', '', '', 'N'"
        << "'1', 'Yukon', 'YT', 'VY1', '01', '02', '', '-1', '', '', 'N'"
        << "'1', 'Prince Edward island', 'PE', 'VY2', '05', '09', '', '-1', '', '', 'N'"
        << "'1', 'Nunavut', 'NU', 'VY0', '02', '04', '', '-1', '', '', 'N'"
        << "'1', 'Nunavut', 'NU', 'VY0', '02', '09', '', '-1', '', '', 'N'"
    ;
    return addValues(rows, values);
}

bool DB_ADIF_Primary_Subdvisions_data::add_USA_6()
{
    // This function contains the Primary Subdivision data for Alaska
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values << "'6', 'Alaska', 'AK', 'KL7', '1', '1', '', '-1', '', '', 'N'";

    return addValues(rows, values);
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

bool DB_ADIF_Primary_Subdvisions_data::add_EA8_29()
{
    // This function contains the Primary Subdivision data for Canary island
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values
        << "'29', 'Las Palmas', 'GC', 'EA8', '33', '36', 'Islas Canarias', '-1', '', '', 'N'"
        << "'29', 'Tenerife', 'TF', 'EA8', '33', '36', 'Islas Canarias', '-1', '', '', 'N'"
        ;

    return addValues(rows, values);
}

bool DB_ADIF_Primary_Subdvisions_data::add_EA9_32()
{
    // This function contains the Primary Subdivision data for Ceuta & Melilla
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values
        << "'32', 'Ceuta', 'CE', 'EA9', '33', '37', 'Ceuta Y Melilla', '-1', '', '', 'N'"
        << "'32', 'Melilla', 'ML', 'EA9', '33', '37', 'Ceuta y Melilla', '-1', '', '', 'N'"
        ;

    return addValues(rows, values);
}

bool DB_ADIF_Primary_Subdvisions_data::add_Mexico_50()
{
    // This function contains the Primary Subdivision data for Mexico
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values
        << "'50', 'Colima', 'COL', 'XE1', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Distrito Federal', 'DF', 'XE1', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Estado de Mexico', 'EMX', 'XE', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Guanajuato', 'GTO', 'XE1', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Hidalgo', 'HGO', 'XE1', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Jalisco', 'JAL', 'XE1', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Michoacan', 'MIC', 'XE1', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Morelos', 'MOR', 'XE1', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Nayarit', 'NAY', 'XE1', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Puebla', 'PUE', 'XE1', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Queretaro', 'QRO', 'XE1', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Tlaxcala', 'TLX', 'XE1', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Veracruz', 'VER', 'XE1', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Aguascalientes', 'AGS', 'XE2', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Baja California', 'BC', 'XE2', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Baja California Sur', 'BCS', 'XE2', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Chihuahua', 'CHH', 'XE2', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Coahuila', 'COA', 'XE2', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Durango', 'DGO', 'XE2', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Nuevo Leon', 'NL', 'XE2', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'San Luis Potosi', 'SLP', 'XE2', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Sinaloa', 'SIN', 'XE2', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Sonora', 'SON', 'XE2', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Tamaulipas', 'TMS', 'XE2', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Zacatecas', 'ZAC', 'XE2', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Campeche', 'CAM', 'XE3', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Chiapas', 'CHS', 'XE3', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Guerrero', 'GRO', 'XE', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Oaxaca', 'OAX', 'XE3', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Quintana Roo', 'QTR', 'XE3', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Tabasco', 'TAB', 'XE3', '06', '10', '', '-1', '', '', 'N'"
        << "'50', 'Yucatan', 'YUC', 'XE3', '06', '10', '', '-1', '', '', 'N'"
        ;
    return addValues(rows, values);
}

bool DB_ADIF_Primary_Subdvisions_data::add_Argentina_100()
{
    // This function contains the Primary Subdivision data for Mexico
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values
        << "'100', 'Buenos Aires City', 'C', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Provincia de Buenos Aires', 'B', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Santa Fe', 'S', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Chaco', 'H', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Formosa', 'P', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Cordoba', 'X', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Misiones', 'N', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Entre Rios', 'E', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Tucuman', 'T', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Corrientes', 'W', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Mendoza', 'M', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Santiago del Estero', 'G', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Salta', 'A', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'San Juan', 'J', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'San Luis', 'D', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Catamarca', 'K', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'La Rioja', 'F', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Jujuy', 'Y', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'La Pampa', 'L', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Rio Negro', 'R', 'LU', '13', '14', '', '-1', '', '', 'N'"
        << "'100', 'Chubut', 'U', 'LU', '13', '16', '', '-1', '', '', 'N'"
        << "'100', 'Santa Cruz', 'Z', 'LU', '13', '16', '', '-1', '', '', 'N'"
        << "'100', 'Tierra del Fuego', 'V', 'LU', '13', '16', '', '-1', '', '', 'N'"
        << "'100', 'Neuquen', 'Q', 'LU', '13', '14', '', '-1', '', '', 'N'"
        ;
    return addValues(rows, values);
}

bool DB_ADIF_Primary_Subdvisions_data::add_Brazil_108()
{
    // This function contains the Primary Subdivision data for Mexico
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values
        << "'108', 'Espirito Santo', 'ES', 'PP1', '11', '15', '', '-1', '', '', 'N'"
        << "'108', 'Goias', 'GO', 'PP2', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Santa Catarina', 'SC', 'PP5', '11', '15', '', '-1', '', '', 'N'"
        << "'108', 'Sergipe', 'SE', 'PP6', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Alagoas', 'AL', 'PP7', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Amazonas', 'AM', 'PP8', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Tocantins', 'TO', 'PQ2', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Amapa', 'AP', 'PQ8', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Paraiba', 'PB', 'PR7', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Maranhao', 'MA', 'PR8', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Rio Grande do Norte', 'RN', 'PS7', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Piuai', 'PI', 'PS8', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Brasila', 'DF', 'PT2', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Ceara', 'CE', 'PT7', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Acre', 'AC', 'PT8', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Mato Grosso do Sul', 'MS', 'PT9', '11', '15', '', '-1', '', '', 'N'"
        << "'108', 'Roraima', 'RR', 'PV8', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Rondonia', 'RO', 'PW8', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Rio de Janeiro', 'RJ', 'PY1', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Sao Paulo', 'SP', 'PY2', '11', '15', '', '-1', '', '', 'N'"
        << "'108', 'Rio Grande do Soul', 'RS', 'PY3', '11', '15', '', '-1', '', '', 'N'"
        << "'108', 'Minas Gerais', 'MG', 'PY4', '11', '15', '', '-1', '', '', 'N'"
        << "'108', 'Parana', 'PR', 'PY5', '11', '15', '', '-1', '', '', 'N'"
        << "'108', 'Bahia', 'BA', 'PY6', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Pernambuco', 'PE', 'PY7', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Para', 'PA', 'PY7', '11', '13', '', '-1', '', '', 'N'"
        << "'108', 'Mato Grosso', 'MT', 'PY9', '11', '13', '', '-1', '', '', 'N'"
        ;
    return addValues(rows, values);
}

bool DB_ADIF_Primary_Subdvisions_data::add_USA_110()
{
    // This function contains the Primary Subdivision data for Hawaii
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values << "'110', 'Hawaii', 'HI', 'KH6', '31', '61', '', '-1', '', '', 'N'";

    return addValues(rows, values);
}

bool DB_ADIF_Primary_Subdvisions_data::add_CT_149()
{
    // This function contains the Primary Subdivision data for Azores Is.
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values
        << "'149', 'Açores', 'AC', 'CU', '14', '36', '', '-1', '', '', 'N'"
        ;

    return addValues(rows, values);
}

bool DB_ADIF_Primary_Subdvisions_data::add_CT_256()
{
    // This function contains the Primary Subdivision data for Madeira Is.
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values
        << "'256', 'Madeira', 'MD', 'CT9', '33', '36', '', '-1', '', '', 'N'"
        ;

    return addValues(rows, values);
}

bool DB_ADIF_Primary_Subdvisions_data::add_CT_272()
{
    // This function contains the Primary Subdivision data for Portugal
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values
        << "'272', 'Aveiro', 'AV', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Beja', 'BJ', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Braga', 'BR', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Bragança', 'BG', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Castelo Branco', 'CB', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Coimbra', 'CO', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Evora', 'EV', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Faro', 'FR', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Guarda', 'GD', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Leiria', 'LR', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Lisboa', 'LX', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Portoalegre', 'PG', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Porto', 'PT', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Santarem', 'SR', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Setubal', 'ST', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Viana do Castelo', 'VC', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Vila Real', 'VR', 'CT', '14', '37', '', '-1', '', '', 'N'"
        << "'272', 'Viseu', 'VS', 'CT', '14', '37', '', '-1', '', '', 'N'"
        ;

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
    values
        << "'281', 'Avila', 'AV', 'EA1', '14', '37', 'Castilla y León', '-1', '', '', 'N'"
        << "'281', 'Burgos', 'BU', 'EA1', '14', '37', 'Castilla y León', '-1', '', '', 'N'"
        << "'281', 'A Coruña', 'C', 'EA1', '14', '37', 'Galicia', '-1', '', '', 'N'"
        << "'281', 'Leon', 'LE', 'EA1', '14', '37', 'Castilla y León', '-1', '', '', 'N'"
        << "'281', 'La Rioja', 'LO', 'EA1', '14', '37', 'La Rioja', '-1', '', '', 'N'"
        << "'281', 'Lugo', 'LU', 'EA1', '14', '37', 'Galicia', '-1', '', '', 'N'"
        << "'281', 'Asturias', 'O', 'EA1', '14', '37', 'Asturias', '-1', '', '', 'N'"
        << "'281', 'Ourense', 'OU', 'EA1', '14', '37', 'Galicia', '-1', '', '', 'N'"
        << "'281', 'Palencia', 'P', 'EA1', '14', '37', 'Castilla y León', '-1', '', '', 'N'"
        << "'281', 'Pontevedra', 'PO', 'EA1', '14', '37', 'Galicia', '-1', '', '', 'N'"
        << "'281', 'Cantabria', 'S', 'EA1', '14', '37', 'Cantabria', '-1', '', '', 'N'"
        << "'281', 'Salamanca', 'SA', 'EA1', '14', '37', 'Castilla y León', '-1', '', '', 'N'"
        << "'281', 'Segovia', 'SG', 'EA1', '14', '37', 'Castilla y León', '-1', '', '', 'N'"
        << "'281', 'Soria', 'SO', 'EA1', '14', '37', 'Castilla y León', '-1', '', '', 'N'"
        << "'281', 'Valladolid', 'VA', 'EA1', '14', '37', 'Castilla y León', '-1', '', '', 'N'"
        << "'281', 'Zamora', 'ZA', 'EA1', '14', '37', 'Castilla y León', '-1', '', '', 'N'"
        << "'281', 'Vizcaya', 'BI', 'EA2', '14', '37', 'Pais Vasco', '-1', '', '', 'N'"
        << "'281', 'Huesca', 'HU', 'EA2', '14', '37', 'Aragon', '-1', '', '', 'N'"
        << "'281', 'Navarra', 'NA', 'EA2', '14', '37', 'Navarra', '-1', '', '', 'N'"
        << "'281', 'Guipuzcoa', 'SS', 'EA2', '14', '37', 'Pais Vasco', '-1', '', '', 'N'"
        << "'281', 'Teruel', 'TE', 'EA2', '14', '37', 'Aragon', '-1', '', '', 'N'"
        << "'281', 'Alava', 'VI', 'EA2', '14', '37', 'Pais Vasco', '-1', '', '', 'N'"
        << "'281', 'Zaragoza', 'Z', 'EA2', '14', '37', 'Aragon', '-1', '', '', 'N'"
        << "'281', 'Barcelona', 'B', 'EA3', '14', '37', 'Cataluña', '-1', '', '', 'N'"
        << "'281', 'Girona', 'GI', 'EA3', '14', '37', 'Cataluña', '-1', '', '', 'N'"
        << "'281', 'Lleida', 'L', 'EA3', '14', '37', 'Cataluña', '-1', '', '', 'N'"
        << "'281', 'Tarragona', 'T', 'EA3', '14', '37', 'Cataluña', '-1', '', '', 'N'"
        << "'281', 'Badajoz', 'BA', 'EA4', '14', '37', 'Extremadura', '-1', '', '', 'N'"
        << "'281', 'Caceres', 'CC', 'EA4', '14', '37', 'Extremadura', '-1', '', '', 'N'"
        << "'281', 'Ciudad Real', 'CR', 'EA4', '14', '37', 'Castilla La Mancha', '-1', '', '', 'N'"
        << "'281', 'Cuenca', 'CU', 'EA4', '14', '37', 'Castilla La Mancha', '-1', '', '', 'N'"
        << "'281', 'Guadalajara', 'GU', 'EA4', '14', '37', 'Castilla La Mancha', '-1', '', '', 'N'"
        << "'281', 'Madrid', 'M', 'EA4', '14', '37', 'Madrid', '-1', '', '', 'N'"
        << "'281', 'Toledo', 'TO', 'EA4', '14', '37', 'Castilla La Mancha', '-1', '', '', 'N'"
        << "'281', 'Alicante', 'A', 'EA5', '14', '37', 'Comunidad Valenciana', '-1', '', '', 'N'"
        << "'281', 'Albacete', 'AB', 'EA5', '14', '37', 'Castilla La Mancha', '-1', '', '', 'N'"
        << "'281', 'Castellon', 'CS', 'EA5', '14', '37', 'Comunidad Valenciana', '-1', '', '', 'N'"
        << "'281', 'Valencia', 'V', 'EA5', '14', '37', 'Comunidad Valenciana', '-1', '', '', 'N'"
        << "'281', 'Murcia', 'MU', 'EA5', '14', '37', 'Region de Murcia ', '-1', '', '', 'N'"
        << "'281', 'Almeria', 'AL', 'EA7', '14', '37', 'Andalucia', '-1', '', '', 'N'"
        << "'281', 'Cadiz', 'CA', 'EA7', '14', '37', 'Andalucia', '-1', '', '', 'N'"
        << "'281', 'Cordoba', 'CO', 'EA7', '14', '37', 'Andalucia', '-1', '', '', 'N'"
        << "'281', 'Granada', 'GR', 'EA7', '14', '37', 'Andalucia', '-1', '', '', 'N'"
        << "'281', 'Huelva', 'H', 'EA7', '14', '37', 'Andalucia', '-1', '', '', 'N'"
        << "'281', 'Jaen', 'J', 'EA7', '14', '37', 'Andalucia', '-1', '', '', 'N'"
        << "'281', 'Malaga', 'MA', 'EA7', '14', '37', 'Andalucia', '-1', '', '', 'N'"
        << "'281', 'Sevilla', 'SE', 'EA7', '14', '37', 'Andalucia', '-1', '', '', 'N'"
        ;

    return addValues(rows, values);
}

bool DB_ADIF_Primary_Subdvisions_data::add_USA_291()
{
    // This function contains the Primary Subdivision data for United States of America
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values
        << "'291', 'Connecticut', 'CT', 'K1', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'Maine', 'ME', 'K1', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'Massachusetts', 'MA', 'K1', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'New Hampshire', 'NH', 'K1', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'Rhode Island', 'RI', 'K1', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'Vermont', 'VT', 'K1', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'New Jersey', 'NJ', 'K2', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'New York', 'NY', 'K2', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'Delaware', 'DE', 'K3', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'District of Columbia', 'DC', 'K3', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'Maryland', 'MD', 'K3', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'Pennsylvania', 'PA', 'K3', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'Alabama', 'AL', 'K4', '04', '08', '', '-1', '', '', 'N'"
        << "'291', 'Florida', 'FL', 'K4', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'Georgia', 'GA', 'K4', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'Kentucky', 'KY', 'K4', '04', '08', '', '-1', '', '', 'N'"
        << "'291', 'North Carolina', 'NC', 'K4', '04', '08', '', '-1', '', '', 'N'"
        << "'291', 'South Carolina', 'SC', 'K4', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'Tennessee', 'TN', 'K4', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Tennessee', 'TN', 'K4', '04', '08', '', '-1', '', '', 'N'"
        << "'291', 'Virginia', 'VA', 'K4', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'Arkansas', 'AR', 'K5', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Arkansas', 'AR', 'K5', '04', '08', '', '-1', '', '', 'N'"
        << "'291', 'Lousiana', 'LA', 'K5', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Lousiana', 'LA', 'K5', '04', '08', '', '-1', '', '', 'N'"
        << "'291', 'Mississippi', 'MS', 'K5', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Mississippi', 'MS', 'K5', '04', '08', '', '-1', '', '', 'N'"
        << "'291', 'New Mexico', 'NM', 'K5', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Oklahoma', 'OK', 'K5', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Texas', 'TX', 'K5', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'California', 'CA', 'K6', '03', '06', '', '-1', '', '', 'N'"
        << "'291', 'Arizona', 'AZ', 'K7', '03', '06', '', '-1', '', '', 'N'"
        << "'291', 'Arizona', 'AZ', 'K7', '03', '07', '', '-1', '', '', 'N'"
        << "'291', 'Idaho', 'ID', 'K7', '03', '06', '', '-1', '', '', 'N'"
        << "'291', 'Montana', 'MT', 'K7', '04', '06', '', '-1', '', '', 'N'"
        << "'291', 'Montana', 'MT', 'K7', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Nevada', 'NV', 'K7', '03', '06', '', '-1', '', '', 'N'"
        << "'291', 'Oregon', 'OR', 'K7', '03', '06', '', '-1', '', '', 'N'"
        << "'291', 'Utah', 'UT', 'K7', '03', '06', '', '-1', '', '', 'N'"
        << "'291', 'Utah', 'UT', 'K7', '03', '07', '', '-1', '', '', 'N'"
        << "'291', 'Washington', 'WA', 'K7', '03', '06', '', '-1', '', '', 'N'"
        << "'291', 'Wyoming', 'WY', 'K7', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Michigan', 'MI', 'K8', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Michigan', 'MI', 'K8', '04', '08', '', '-1', '', '', 'N'"
        << "'291', 'Ohio', 'OH', 'K8', '04', '08', '', '-1', '', '', 'N'"
        << "'291', 'West Virginia', 'WV', 'K8', '05', '08', '', '-1', '', '', 'N'"
        << "'291', 'Illinois', 'IL', 'K9', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Illinois', 'IL', 'K9', '04', '08', '', '-1', '', '', 'N'"
        << "'291', 'Indiana', 'IN', 'K9', '04', '08', '', '-1', '', '', 'N'"
        << "'291', 'Wisconsin', 'WI', 'K9', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Wisconsin', 'WI', 'K9', '04', '08', '', '-1', '', '', 'N'"
        << "'291', 'Colorado', 'CO', 'K0', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Iowa', 'IA', 'K0', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Kansas', 'KS', 'K0', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Minnesota', 'MN', 'K0', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Minnesota', 'MN', 'K0', '04', '08', '', '-1', '', '', 'N'"
        << "'291', 'Missouri', 'MO', 'K0', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'Missouri', 'MO', 'K0', '04', '08', '', '-1', '', '', 'N'"
        << "'291', 'Nebraska', 'NE', 'K0', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'North dakota', 'ND', 'K0', '04', '07', '', '-1', '', '', 'N'"
        << "'291', 'South Dakota', 'SD', 'K0', '04', '07', '', '-1', '', '', 'N'"
           ;

    return addValues(rows, values);
}

bool DB_ADIF_Primary_Subdvisions_data::add_Japan_339()
{
    // This function contains the Primary Subdivision data for Japan
    // Source of data is https://www.adif.org (version 3.1.4)
    // Last modification 20240620

    QString rows = QString("dxcc, name, shortname, prefix, cqz, ituz, regionalgroup, regionalid, start_date, end_date, deleted");
    QStringList values;
    values.clear();
    values
        << "'339', 'Chiba', '12', 'JA1', '25', '45', 'Kanto', '-1', '', '', 'N'"
        << "'339', 'Gunma', '16', 'JA1', '25', '45', 'Kanto', '-1', '', '', 'N'"
        << "'339', 'Ibaraki', '14', 'JA1', '25', '45', 'Kanto', '-1', '', '', 'N'"
        << "'339', 'Kanagawa', '11', 'JA1', '25', '45', 'Kanto', '-1', '', '', 'N'"
        << "'339', 'Saitama', '13', 'JA1', '25', '45', 'Kanto', '-1', '', '', 'N'"
        << "'339', 'Tochigi', '15', 'JA1', '25', '45', 'Kanto', '-1', '', '', 'N'"
        << "'339', 'Tokyo', '10', 'JA1', '25', '45', 'Kanto', '-1', '', '', 'N'"
        << "'339', 'Yamanashi', '17', 'JA1', '25', '45', 'Kanto', '-1', '', '', 'N'"
        << "'339', 'Aichi', '20', 'JA2', '25', '45', 'Tokai', '-1', '', '', 'N'"
        << "'339', 'Gifu', '19', 'JA2', '25', '45', 'Tokai', '-1', '', '', 'N'"
        << "'339', 'Mie', '21', 'JA2', '25', '45', 'Tokai', '-1', '', '', 'N'"
        << "'339', 'Shizuoka', '18', 'JA2', '25', '45', 'Tokai', '-1', '', '', 'N'"
        << "'339', 'Hyogo', '27', 'JA3', '25', '45', 'Kansai', '-1', '', '', 'N'"
        << "'339', 'Kyoto', '22', 'JA3', '25', '45', 'Kansai', '-1', '', '', 'N'"
        << "'339', 'Nara', '24', 'JA3', '25', '45', 'Kansai', '-1', '', '', 'N'"
        << "'339', 'Osaka', '25', 'JA3', '25', '45', 'Kansai', '-1', '', '', 'N'"
        << "'339', 'Shiga', '23', 'JA3', '25', '45', 'Kansai', '-1', '', '', 'N'"
        << "'339', 'Wakayama', '26', 'JA3', '25', '45', 'Kansai', '-1', '', '', 'N'"
        << "'339', 'Hiroshima', '35', 'JA4', '25', '45', 'Chugoku', '-1', '', '', 'N'"
        << "'339', 'Okayama', '31', 'JA4', '25', '45', 'Chugoku', '-1', '', '', 'N'"
        << "'339', 'Shimane', '32', 'JA4', '25', '45', 'Chugoku', '-1', '', '', 'N'"
        << "'339', 'Tottori', '34', 'JA4', '25', '45', 'Chugoku', '-1', '', '', 'N'"
        << "'339', 'Yamaguchi', '33', 'JA4', '25', '45', 'Chugoku', '-1', '', '', 'N'"
        << "'339', 'Ehime', '38', 'JA5', '25', '45', 'Shikoku', '-1', '', '', 'N'"
        << "'339', 'Kagawa', '36', 'JA5', '25', '45', 'Shikoku', '-1', '', '', 'N'"
        << "'339', 'Kochi', '39', 'JA5', '25', '45', 'Shikoku', '-1', '', '', 'N'"
        << "'339', 'Tokushima', '37', 'JA5', '25', '45', 'Shikoku', '-1', '', '', 'N'"
        << "'339', 'Fukuoka', '40', 'JA6', '25', '45', 'Kyushu', '-1', '', '', 'N'"
        << "'339', 'Kagoshima', '46', 'JA6', '25', '45', 'Kyushu', '-1', '', '', 'N'"
        << "'339', 'Kumamoto', '43', 'JA6', '25', '45', 'Kyushu', '-1', '', '', 'N'"
        << "'339', 'Miyazaki', '45', 'JA6', '25', '45', 'Kyushu', '-1', '', '', 'N'"
        << "'339', 'Nagasaki', '42', 'JA6', '25', '45', 'Kyushu', '-1', '', '', 'N'"
        << "'339', 'Oita', '44', 'JA6', '25', '45', 'Kyushu', '-1', '', '', 'N'"
        << "'339', 'Okinawa', '47', 'JS6', '25', '45', 'Kyushu', '-1', '', '', 'N'"
        << "'339', 'Saga', '41', 'JA6', '25', '45', 'Kyushu', '-1', '', '', 'N'"
        << "'339', 'Akita', '04', 'JA7', '25', '45', 'Tohoku', '-1', '', '', 'N'"
        << "'339', 'Aomori', '02', 'JA7', '25', '45', 'Tohoku', '-1', '', '', 'N'"
        << "'339', 'Fukushima', '07', 'JA7', '25', '45', 'Tohoku', '-1', '', '', 'N'"
        << "'339', 'Iwate', '03', 'JA7', '25', '45', 'Tohoku', '-1', '', '', 'N'"
        << "'339', 'Miyagi', '06', 'JA7', '25', '45', 'Tohoku', '-1', '', '', 'N'"
        << "'339', 'Yamagata', '05', 'JA7', '25', '45', 'Tohoku', '-1', '', '', 'N'"
        << "'339', 'Hokkaido', '01', 'JA8', '25', '45', 'Hokkaido', '-1', '', '', 'N'"
        << "'339', 'Fukui', '29', 'JA9', '25', '45', 'Hokuriku', '-1', '', '', 'N'"
        << "'339', 'Ishikawa', '30', 'JA9', '25', '45', 'Hokuriku', '-1', '', '', 'N'"
        << "'339', 'Toyama', '28', 'JA9', '25', '45', 'Hokuriku', '-1', '', '', 'N'"
        << "'339', 'Nagano', '09', 'JA0', '25', '45', 'Shinestu', '-1', '', '', 'N'"
        << "'339', 'Niigata', '08', 'JA0', '25', '45', 'Shinestu', '-1', '', '', 'N'"
        ;

    return addValues(rows, values);
}

