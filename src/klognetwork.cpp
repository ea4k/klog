#include <QtGui>
#include <QtNetwork>

#include "klognetwork.h"
//#include "ui_authenticationdialog.h"

KLogNetwork::KLogNetwork(QWidget *parent)
     : QDialog(parent){
//qDebug() << "KLogNetwork::KLogNetwork" << endl;


  urlLineEdit = new QLineEdit("http://www.country-files.com/cty/cty.dat");
  urlLineEdit->setReadOnly(true);
  urlLabel = new QLabel(i18n("&URL:"));
  urlLabel->setBuddy(urlLineEdit);
  statusLabel = new QLabel(i18n("You are going to download the cty.dat file from:"));
  statusLabel2 = new QLabel(i18n("Do you want to continue?"));
  downloadButton = new QPushButton(i18n("Yes"));
  downloadButton->setDefault(true);
  quitButton = new QPushButton(i18n("No"));
  quitButton->setAutoDefault(false);

  buttonBox = new QDialogButtonBox;
  buttonBox->addButton(downloadButton, QDialogButtonBox::ActionRole);
  buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

  progressDialog = new QProgressDialog(this);

  http = new QHttp(this);

  //connect(urlLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableDownloadButton()));
  connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
  connect(http, SIGNAL(dataReadProgress(int, int)), this, SLOT(updateDataReadProgress(int, int)));
  connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
  connect(http, SIGNAL(authenticationRequired(const QString &, quint16, QAuthenticator *)), this, SLOT(slotAuthenticationRequired(const QString &, quint16, QAuthenticator *)));
#ifndef QT_NO_OPENSSL
  connect(http, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(sslErrors(const QList<QSslError> &)));
#endif
  connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));
  connect(downloadButton, SIGNAL(clicked()), this, SLOT(downloadFile()));
  //connect(quitButton, SIGNAL(clicked()), this, SLOT(done(1)));
  connect(quitButton, SIGNAL(clicked()), this, SLOT(reject() ) );

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(urlLabel);
  topLayout->addWidget(urlLineEdit);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(statusLabel);
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(statusLabel2);
  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);

  setWindowTitle(i18n("KLog: CTY.DAT download"));
  urlLineEdit->setFocus();
 }

void KLogNetwork::downloadFile() {
  //qDebug() << "KLogNetwork::downloadFile" << endl;
  //http://www.country-files.com/cty/cty.dat
    // QUrl url(urlLineEdit->text());
     QUrl url("http://www.country-files.com/cty/cty.dat");
     QFileInfo fileInfo(url.path());
     QString fileName = fileInfo.fileName();
     
//      if (fileName.isEmpty())
//          fileName = "index.html";

     if (QFile::exists(fileName)) {
         if (QMessageBox::question(this, i18n("KLog: File exists!"),
                                   i18n("There already exists a file called %1 in "
                                      "the current directory. Overwrite?").arg(fileName),
                                   QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
             == QMessageBox::No)
             return;
         QFile::remove(fileName);
     }

     file = new QFile(fileName);
     if (!file->open(QIODevice::WriteOnly)) {
         QMessageBox::information(this, i18n("KLog: Can't save the file"),
                                  i18n("Unable to save the file %1: %2.")
                                  .arg(fileName).arg(file->errorString()));
         delete file;
         file = 0;
         return;
     }

     QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
     http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

     if (!url.userName().isEmpty())
         http->setUser(url.userName(), url.password());

     httpRequestAborted = false;
     QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
     if (path.isEmpty())
         path = "/";
     httpGetId = http->get(path, file);

     progressDialog->setWindowTitle(i18n("KLog: Downloading..."));
     progressDialog->setLabelText(i18n("Downloading %1.").arg(fileName));
    // downloadButton->setEnabled(false);
 }

 void KLogNetwork::cancelDownload() {
   //qDebug() << "KLogNetwork::cancelDownload" << endl;
     statusLabel->setText(i18n("Download canceled."));
     httpRequestAborted = true;
     http->abort();
     downloadButton->setEnabled(true);
 }

 void KLogNetwork::httpRequestFinished(int requestId, bool error){
  //qDebug() << "KLogNetwork::httpRequestFinished" << endl;
     if (requestId != httpGetId)
         return;
     if (httpRequestAborted) {
         if (file) {
             file->close();
             file->remove();
             delete file;
             file = 0;
         }

         progressDialog->hide();
         return;
     }

     if (requestId != httpGetId)
         return;

     progressDialog->hide();
     file->close();

     if (error) {
         file->remove();
         QMessageBox::information(this, i18n("KLog: Download failed"),
                                  i18n("Download failed: %1.")
                                  .arg(http->errorString()));
     } else {
         QString fileName = QFileInfo(QUrl(urlLineEdit->text()).path()).fileName();
         statusLabel->setText(i18n("Downloaded %1 to the KLog directory.").arg(fileName));
	 
	 
	  QMessageBox msgBox;
    msgBox.setWindowTitle(i18n("KLog: cty.dat downloaded"));
    msgBox.setText(i18n("KLog has downloaded the cty.dat file successfully.\n"));
  //  msgBox.setInformativeText("Do you want to download the cty.dat file now?");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    
    switch (ret) {
      case QMessageBox::Ok:
      done(1);
      break;
      default:
       // should never be reached
      break;
    }
	 
	 
	 
	 
	 
     }

     downloadButton->setEnabled(true);
     delete file;
     file = 0;
 }

 void KLogNetwork::readResponseHeader(const QHttpResponseHeader &responseHeader){
  //qDebug() << "KLogNetwork::readResponseHeader" << endl;
     switch (responseHeader.statusCode()) {
     case 200:                   // Ok
     case 301:                   // Moved Permanently
     case 302:                   // Found
     case 303:                   // See Other
     case 307:                   // Temporary Redirect
         // these are not error conditions
         break;

     default:
         QMessageBox::information(this, i18n("KLog: Download failed"),
                                  i18n("Download failed: %1.")
                                  .arg(responseHeader.reasonPhrase()));
         httpRequestAborted = true;
         progressDialog->hide();
         http->abort();
     }
 }

 void KLogNetwork::updateDataReadProgress(int bytesRead, int totalBytes){
  // qDebug() << "KLogNetwork::updateDataReadProgress" << endl;
     if (httpRequestAborted)
         return;

     progressDialog->setMaximum(totalBytes);
     progressDialog->setValue(bytesRead);
 }

//  void KLogNetwork::enableDownloadButton(){
//    qDebug() << "KLogNetwork::enableDownloadButton" << endl;
//      downloadButton->setEnabled(!urlLineEdit->text().isEmpty());
//  }

 void KLogNetwork::slotAuthenticationRequired(const QString &hostName, quint16, QAuthenticator *authenticator){
   //qDebug() << "KLogNetwork::slotAuthenticationRequired" << endl;
   // Add a message requesting the authentication
   
   
   /*
     QDialog dlg;
     Ui::Dialog ui;
     ui.setupUi(&dlg);
     dlg.adjustSize();
     ui.siteDescription->setText(i18n("%1 at %2").arg(authenticator->realm()).arg(hostName));

     if (dlg.exec() == QDialog::Accepted) {
         authenticator->setUser(ui.userEdit->text());
         authenticator->setPassword(ui.passwordEdit->text());
     }
     */
 }

 #ifndef QT_NO_OPENSSL
 void KLogNetwork::sslErrors(const QList<QSslError> &errors){
//qDebug() << "KLogNetwork::sslErrors" << endl;
     QString errorString;
     foreach (const QSslError &error, errors) {
         if (!errorString.isEmpty())
             errorString += ", ";
         errorString += error.errorString();
     }

     if (QMessageBox::warning(this, i18n("KLog: SSL error"),
                              i18n("One or more SSL errors has occurred: %1").arg(errorString),
                              QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
         http->ignoreSslErrors();
     }
 }
 #endif
 