#ifndef STARTWIZARD_H
#define STARTWIZARD_H
/***************************************************************************
                          startwizard.h  -  description
                             -------------------
    begin                : feb 2015
    copyright            : (C) 2015 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include <QWizard>
#include <QWizardPage>
#include <QtWidgets>
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
    CTYPage(const QString _kontestDir, const QString _version, QWidget *parent = 0);
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

