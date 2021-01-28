#ifndef KLOGDEFINITIONS_H
#define KLOGDEFINITIONS_H
#include <QString>
enum ExportMode {ModeLotW, ModeADIF, ModeClubLog, ModeEQSL, ModeQRZ};
enum OnLineProvider {ClubLog, LoTW, eQSL, QRZ}; //, HamQTH, HRDLog
enum OnlineErrorCode {Ok, Fail};
enum OnlineErrorReason {Other, Auth, DupeQSO, WrongLogBook};
enum DebugLogLevel {Info, Debug};

struct EntityData { // Used to pass a list of data from World to dxccstatuswidget
  int dxcc;
  QString mainprefix;
  QString name;
  QString isoname;

} ;

#endif // KLOGDEFINITIONS_H
