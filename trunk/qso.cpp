#include "qso.h"

QSO::QSO()
{
}

QSO::~QSO()
{
}

void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

void QSO::clear()
{
    address.clear();
    address_int.clear();
    call.clear();
    check.clear();
    contestClass.clear();
    comment.clear();
    comment_intl.clear();
    contacted_op.clear();
    contest_id.clear();
    email.clear();
    eq_call.clear();
    fists.clear();
    my_fists.clear();
    guest_op.clear();
    ms_shower.clear();
    my_city.clear();
    my_city_intl.clear();
    name.clear();
    name_intl.clear();
    my_name.clear();
    my_name_intl.clear();
    my_postal_code.clear();
    my_postal_code_intl.clear();
    my_rig.clear();
    my_rig_intl.clear();
    my_sig.clear();
    my_sig_intl.clear();
    my_sig_info.clear();
    my_sig_info_intl.clear();
    my_street.clear();
    my_street_intl.clear();
    notes.clear();
    notes_intl.clear();
    operator_call.clear();
    owner_callsign.clear();
    pfx.clear();
    precedence.clear();
    public_key.clear();
    qslmsg.clear();
    qslmsg_intl.clear();
    qsl_via.clear();
    qth.clear();
    qth_intl.clear();
    rig.clear();
    rig_intl.clear();
    rst_rcvd.clear();
    rst_sent.clear();
    sat_mode.clear();
    sat_name.clear();
    sig.clear();
    sig_intl.clear();
    sig_info.clear();
    sig_info_intl.clear();
    skcc.clear();
    srx_string.clear();
    stx_string.clear();
    station_callsign.clear();
    web.clear();

    //BOOLEAN
    force_init = QChar::Null;
    qso_random = QChar::Null;
    SWL = QChar::Null;

    //Numbers
    age.clear();
    a_index.clear();
    ant_az.clear();
    ant_el.clear();
    cqz.clear();
    distance.clear();
    freq.clear();
    freq_rx.clear();
    ituz.clear();
    k_index.clear();
    max_bursts.clear();
    my_cq_zone.clear();
    nr_bursts.clear();
    nr_pings.clear();
    rx_pwr.clear();
    sfi.clear();
    srx.clear();
    stx.clear();
    ten_ten.clear();
    tx_pwr.clear();

    //Dates
    qso_date.clear();
    qso_date_off.clear();
    clublog_qso_upload_date.clear();
    eqsl_qslrdate.clear();
    eqsl_qslsdate.clear();
    hrdlog_qso_upload_date.clear();
    lotw_qslrdate.clear();
    lotw_qslsdate.clear();
    qrzcom_qso_upload_date.clear();
    qslrdate.clear();
    qslsdate.clear();

    //TIME
    time_off.clear();
    time_on.clear();
    //QSL
    eqsl_qsl_rcvd.clear();
    eqsl_qsl_sent.clear();

    //Enums
    mode.clear();
    submode.clear();
    qso_complete.clear();
    ant_path.clear();
    arrl_sect.clear();
    award_submitted.clear();
    award_granted.clear();
    band.clear();
    band_rx.clear();
    cnty.clear();
    my_cnty.clear();
    my_usaca_counties.clear();
    usaca_couties.clear();
    cont.clear();
    credit_submitted.clear();
    credit_granted.clear();
    dxcc.clear();
    my_dxcc.clear();
    my_vucc_grids.clear();
    vucc_grids.clear();
    prop_mode.clear();

    //GRIDSQUARE
    gridsquare.clear();
    my_gridsquare.clear();

    //Location
    lat.clear();
    lon.clear();

    //QSO UPLOAD STATUS
    clublog_qso_upload_status.clear();
    hrdlog_qso_upload_status.clear();
    qrzcom_qso_upload_status.clear();

    //IOTA
    iota.clear();
    iota_island_id.clear();
    my_iota.clear();
    my_iota_island_id.clear();

    //QSL_RCVD
    lotw_qsl_rcvd.clear();
    qsl_rcvd.clear();

    //QSL_SENT
    lotw_qsl_sent.clear();
    qsl_sent.clear();

    //QSL-VIA
    qsl_rcvd_via.clear();
    qsl_sent_via.clear();

    //SOTAREF
    my_sota_ref.clear();
    sota_ref.clear();

    //COUNTRY
    my_country.clear();
    my_country_intl.clear();
    country.clear();
    country_intl.clear();
    my_state.clear();
    state.clear();
}

void QSO::setAdress(const QString& t)
{
    address = t;
}
QString QSO::getAdress()
{
    return address;
}

void QSO::setAddress_int(const QString& t)
{
    address_int = t;
}
QString QSO::getAddress_int()
{
    return address_int;
}

void QSO::setCall(const QString& t)
{
    call = t;
}
QString QSO::getCall()
{
    return call ;
}


void QSO::setCheck(const QString& t)
{
  check  = t;
}
QString QSO::getCheck()
{
    return check;
}


void QSO::setContestClass(const QString& t)
{
   contestClass = t;
}
QString QSO::getContestClass()
{
    return contestClass;
}


void QSO::setComment(const QString& t)
{
  comment  = t;
}
QString QSO::getComment()
{
    return comment;
}


void QSO::setComment_intl(const QString& t)
{
   comment_intl = t;
}
QString QSO::getcomment_intl()
{
    return comment_intl;
}


void QSO::setContacted_op(const QString& t)
{
   contacted_op = t;
}
QString QSO::getContacted_op()
{
    return contacted_op;
}


void QSO::setContest_id(const QString& t)
{
  contest_id  = t;
}
QString QSO::getContest_id()
{
    return contest_id;
}

void QSO::setEmail(const QString& t)
{
    email = t;
}
QString QSO::getEmail()
{
    return email;
}

void QSO::set(const QString& t)
{
   eq_call = t;
}
QString QSO::get()
{
    return eq_call;
}

void QSO::set(const QString& t)
{
   fists = t;
}
QString QSO::get()
{
    return fists;
}

void QSO::set(const QString& t)
{
   my_fists = t;
}
QString QSO::get()
{
    return my_fists;
}

void QSO::set(const QString& t)
{
   guest_op = t;
}
QString QSO::get()
{
    return guest_op;
}

void QSO::set(const QString& t)
{
   ms_shower = t;
}
QString QSO::get()
{
    return ms_shower;
}

void QSO::set(const QString& t)
{
   my_city = t;
}
QString QSO::get()
{
    return my_city;
}


void QSO::set(const QString& t)
{
  my_city_intl  = t;
}
QString QSO::get()
{
    return my_city_intl;
}

void QSO::set(const QString& t)
{
   name = t;
}
QString QSO::get()
{
    return name;
}

void QSO::set(const QString& t)
{
  name_intl  = t;
}
QString QSO::get()
{
    return name_intl;
}

void QSO::set(const QString& t)
{
  my_name  = t;
}
QString QSO::get()
{
    return my_name;
}

void QSO::set(const QString& t)
{
  my_name_intl  = t;
}
QString QSO::get()
{
    return my_name_intl;
}

void QSO::set(const QString& t)
{
  my_postal_code  = t;
}
QString QSO::get()
{
    return my_postal_code;
}

void QSO::set(const QString& t)
{
  my_postal_code_intl  = t;
}
QString QSO::get()
{
    return my_postal_code_intl;
}


void QSO::set(const QString& t)
{
   my_rig = t;
}
QString QSO::get()
{
    return my_rig;
}

void QSO::set(const QString& t)
{
   my_rig_intl = t;
}
QString QSO::get()
{
    return my_rig_intl;
}

void QSO::set(const QString& t)
{
  my_sig  = t;
}
QString QSO::get()
{
    return my_sig;
}

void QSO::set(const QString& t)
{
   my_sig_intl = t;
}
QString QSO::get()
{
    return my_sig_intl;
}

void QSO::set(const QString& t)
{
   my_sig_info = t;
}
QString QSO::get()
{
    return my_sig_info;
}

void QSO::set(const QString& t)
{
   my_sig_info_intl = t;
}
QString QSO::get()
{
    return my_sig_info_intl;
}

void QSO::set(const QString& t)
{
   my_street = t;
}
QString QSO::get()
{
    return my_street;
}


void QSO::set(const QString& t)
{
  my_street_intl  = t;
}
QString QSO::get()
{
    return my_street_intl;
}

void QSO::set(const QString& t)
{
   notes = t;
}
QString QSO::get()
{
    return notes;
}

void QSO::set(const QString& t)
{
  notes_intl  = t;
}
QString QSO::get()
{
    return notes_intl;
}

void QSO::set(const QString& t)
{
  operator_call  = t;
}
QString QSO::get()
{
    return operator_call;
}

void QSO::set(const QString& t)
{
   owner_callsign = t;
}
QString QSO::get()
{
    return owner_callsign;
}

void QSO::setPfx(const QString& t)
{
    pfx = t;
}
QString QSO::getPfx()
{
    return pfx ;
}

void QSO::set(const QString& t)
{
   precedence = t;
}
QString QSO::get()
{
    return precedence;
}


void QSO::set(const QString& t)
{
    public_key = t;
}
QString QSO::get()
{
    return public_key;
}

void QSO::set(const QString& t)
{
  qslmsg  = t;
}
QString QSO::get()
{
    return qslmsg;
}

void QSO::set(const QString& t)
{
  qslmsg_intl  = t;
}
QString QSO::get()
{
    return qslmsg_intl;
}

void QSO::set(const QString& t)
{
  qsl_via  = t;
}
QString QSO::get()
{
    return qsl_via;
}

void QSO::set(const QString& t)
{
  qth  = t;
}
QString QSO::get()
{
    return qth;
}

void QSO::set(const QString& t)
{
  qth_intl  = t;
}
QString QSO::get()
{
    return qth_intl;
}

void QSO::set(const QString& t)
{
  rig  = t;
}
QString QSO::get()
{
    return rig;
}

void QSO::set(const QString& t)
{
  rig_intl  = t;
}
QString QSO::get()
{
    return rig_intl;
}

void QSO::set(const QString& t)
{
  rst_rcvd  = t;
}
QString QSO::get()
{
    return rst_rcvd;
}

void QSO::set(const QString& t)
{
   rst_sent = t;
}
QString QSO::get()
{
    return rst_sent;
}


void QSO::set(const QString& t)
{
  sat_mode  = t;
}
QString QSO::get()
{
    return sat_mode;
}

void QSO::set(const QString& t)
{
   sat_name = t;
}
QString QSO::get()
{
    return sat_name;
}

void QSO::set(const QString& t)
{
   sig = t;
}
QString QSO::get()
{
    return sig;
}

void QSO::set(const QString& t)
{
   sig_intl = t;
}
QString QSO::get()
{
    return sig_intl;
}

void QSO::set(const QString& t)
{
  sig_info  = t;
}
QString QSO::get()
{
    return sig_info;
}

void QSO::set(const QString& t)
{
   sig_info_intl = t;
}
QString QSO::get()
{
    return sig_info_intl;
}

void QSO::set(const QString& t)
{
   skcc = t;
}
QString QSO::get()
{
    return skcc;
}

void QSO::set(const QString& t)
{
   srx_string = t;
}
QString QSO::get()
{
    return srx_string;
}

void QSO::set(const QString& t)
{
   stx_string = t;
}
QString QSO::get()
{
    return stx_string;
}


void QSO::set(const QString& t)
{
   station_callsign = t;
}
QString QSO::get()
{
    return station_callsign;
}

void QSO::set(const QString& t)
{
   web = t;
}
QString QSO::get()
{
    return web;
}


//BOOLEAN
force_init = QChar::Null;
qso_random = QChar::Null;
SWL = QChar::Null;

//Numbers

void QSO::set(const QString& t)
{
  age  = t;
}
QString QSO::get()
{
    return age;
}

void QSO::set(const QString& t)
{
  a_index  = t;
}
QString QSO::get()
{
    return a_index;
}

void QSO::set(const QString& t)
{
   ant_az = t;
}
QString QSO::get()
{
    return ant_az;
}

void QSO::set(const QString& t)
{
  ant_el  = t;
}
QString QSO::get()
{
    return ant_el;
}

void QSO::set(const QString& t)
{
  cqz  = t;
}
QString QSO::get()
{
    return cqz;
}

void QSO::set(const QString& t)
{
  distance  = t;
}
QString QSO::get()
{
    return distance;
}
 freq
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 freq_rx
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 ituz
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 k_index
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 max_bursts
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

my_cq_zone
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 nr_bursts
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 nr_pings
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 rx_pwr
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 sfi
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 srx
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 stx
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 ten_ten
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 tx_pwr
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}


//Dates
qso_date
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 qso_date_off
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 clublog_qso_upload_date
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 eqsl_qslrdate
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 eqsl_qslsdate
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 hrdlog_qso_upload_date
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

lotw_qslrdate
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 lotw_qslsdate
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 qrzcom_qso_upload_date
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 qslrdate
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 qslsdate
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}


//TIME
time_off
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

time_on
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

//QSL
eqsl_qsl_rcvd
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

eqsl_qsl_sent
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}


//Enums
mode
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 submode
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

qso_complete
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

ant_path
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

arrl_sect
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

award_submitted
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

award_granted
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

band
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 band_rx
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

cnty
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 my_cnty
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

my_usaca_counties
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 usaca_couties
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

cont
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

credit_submitted
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

credit_granted
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

dxcc
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 my_dxcc
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

my_vucc_grids
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 vucc_grids
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

prop_mode
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}


//GRIDSQUARE
gridsquare
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 my_gridsquare
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}


//Location
lat
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 lon
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}


//QSO UPLOAD STATUS
clublog_qso_upload_status
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 hrdlog_qso_upload_status
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 qrzcom_qso_upload_status
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}


//IOTA
iota
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 iota_island_id
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 my_iota
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 my_iota_island_id
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}


//QSL_RCVD
lotw_qsl_rcvd
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 qsl_rcvd
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}


//QSL_SENT
lotw_qsl_sent
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 qsl_sent
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}


//QSL-VIA
qsl_rcvd_via
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 qsl_sent_via
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}


//SOTAREF
my_sota_ref
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 sota_ref
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}


//COUNTRY
my_country
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 my_country_intl
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 country
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 country_intl
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}

my_state
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
 state
void QSO::set(const QString& t)
{
    = t;
}
QString QSO::get()
{
    return ;
}
