#ifndef STARTWIZARD_H
#define STARTWIZARD_H

#include <QWizard>
#include <QWizardPage>
#include <QtGui>
//#include <QtWidgets>

#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QProgressDialog>

#include "downloadcty.h"


class FileOrMemoryPage;
class CTYPage;

 class StartWizard : public QWizard
 {
     Q_OBJECT

 public:
     enum { Page_Intro, Page_Lic, Page_Mem, Page_CTY };

    StartWizard(const QString _kontestDir, const QString _softVersion, QWidget *parent = 0);
    void setVersion(QString tversion);
    //~StartWizard();

 protected:

 private slots:
    void slotCancelWizard();
    void slotRunInMemory(bool checked);
    void slotButtonFinishedClicked();

    //void fin();

 private:


   FileOrMemoryPage *fileOrMemoryPage;
   CTYPage *ctyPage;
   QString version;
   bool inMemory;
   QString kontestDir;

};

 class IntroPage : public QWizardPage
   {
       Q_OBJECT
   public:
     IntroPage(QWidget *parent = 0);
     int nextId() const;
   private:
       QLabel *topLabel;
       QTextEdit *welcomeBrowser;
 };


class LicPage : public QWizardPage
  {
      Q_OBJECT
  public:
    LicPage(QWidget *parent = 0);
    void setNextButtonActive(const bool _active);
    int nextId() const;

  private:
      QLabel *topLabel;
      QTextEdit *licenseBrowser;
      QCheckBox *aceptLicCheckBox;
};
/*

class FileOrMemoryPage : public QWizardPage
  {
      Q_OBJECT

  public:
      FileOrMemoryPage(QWidget *parent = 0);
      int nextId() const;


      //int nextId() const;
private slots:
  void slotRunningModeSelectedFile(bool checked);
  void slotRunningModeSelectedMemory(bool checked);
signals:
    void exeInMemory(const bool mem);

  private:

    bool downloadCTYFile();

    QLabel *topLabel;
    QRadioButton *memoryRadioButton;
    QRadioButton *fileRadioButton;


    bool runInMemory;

};
*/
class CTYPage : public QWizardPage
  {
      Q_OBJECT
  public:
    CTYPage(const QString _kontestDir, QWidget *parent = 0);
    void updateProgress (qint64 v,qint64 t);

private slots:
/*
    void slotCTYCheckButtonTogled(int state);
    void slotDownloadCTY(bool checked);
    void slotDownloadFinished();
    void slotValueReturnedFromDownload(int value);

    void slotDownloadButtonClicked();
*/
    void slotUpdateDownloadProgress(qint64 received, qint64 total);
    void slotDownloadButtonClicked();
    void slotIgnoreDownloadButtonClicked();
    void slotDownloadFinished(const int ret);
    void slotDownloadError(const int ret);

signals:
    void downloadTheFileSignal(const bool mem);

  private:
    bool prepareTheDownload();
    bool doTheDownload();
    //bool isComplete();
    bool downloadCTYFile, CTYFileDownloaded;
    int downloadValueResult;
    //QString kontestDir;
    int total;
    //bool completed;

   DownLoadCTY *dl;

      QLabel *topLabel;
      QTextEdit *ctyBrowser;
      QCheckBox *downloadCTYCheckBox;
      QProgressBar *progressBar;
      QPushButton *downloadButton, *ignoreDownloadButton;
      QCheckBox *hiddenCheckBox;
};



#endif // STARTWIZARD_H

