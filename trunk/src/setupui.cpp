#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './setupui.ui'
**
** Created: jue sep 13 18:00:59 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "setupui.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlabel.h>
#include <klineedit.h>
#include <qframe.h>
#include <qcheckbox.h>
#include <kpushbutton.h>
#include <knuminput.h>
#include <kcombobox.h>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <kcolorcombo.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qheader.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

static const unsigned char img0_setupui[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x16,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xc4, 0xb4, 0x6c, 0x3b, 0x00, 0x00, 0x02,
    0x73, 0x49, 0x44, 0x41, 0x54, 0x38, 0x8d, 0x8d, 0x94, 0xb1, 0x6b, 0xdb,
    0x40, 0x14, 0x87, 0x5f, 0xa0, 0x43, 0x46, 0x8f, 0xee, 0x50, 0xc8, 0xda,
    0xd1, 0xa3, 0xfd, 0x1f, 0x94, 0x2e, 0x05, 0xd3, 0x31, 0x43, 0x92, 0xa9,
    0xcd, 0x14, 0x4c, 0x87, 0x10, 0x32, 0x18, 0x27, 0x43, 0xd0, 0x64, 0x90,
    0x37, 0xa5, 0x60, 0xb0, 0x97, 0xe0, 0x2c, 0xc1, 0xcd, 0x10, 0x70, 0x86,
    0x80, 0xb4, 0xd9, 0x43, 0x40, 0x1e, 0x3a, 0x68, 0xf0, 0xa0, 0x82, 0x06,
    0x11, 0x34, 0x68, 0x30, 0x81, 0xaf, 0xc3, 0xdd, 0xd9, 0x39, 0x25, 0x51,
    0x72, 0xcb, 0xd3, 0x3b, 0xdd, 0xfd, 0xf4, 0xdd, 0xef, 0x3d, 0x9d, 0x00,
    0x42, 0x80, 0x00, 0x42, 0x84, 0x1a, 0x33, 0x15, 0xbc, 0x5b, 0x9d, 0x07,
    0x2a, 0xf8, 0x81, 0x2f, 0x4f, 0xd7, 0x97, 0xe5, 0xab, 0x24, 0x9c, 0x2b,
    0xd5, 0xce, 0x6d, 0x0a, 0x80, 0x33, 0xcb, 0x01, 0x70, 0xef, 0xf5, 0x47,
    0xee, 0x62, 0x00, 0xc6, 0x57, 0x3e, 0x4f, 0x45, 0x5e, 0x8b, 0x4a, 0xd4,
    0x6c, 0xd6, 0x64, 0xe3, 0xb9, 0x1d, 0x47, 0xe6, 0x04, 0x26, 0x1f, 0x4e,
    0x78, 0x8b, 0x5c, 0xfc, 0x20, 0x54, 0x8b, 0xf5, 0xa6, 0x30, 0xd3, 0x31,
    0xd7, 0xc7, 0x8f, 0x75, 0x5c, 0xa8, 0x89, 0xc1, 0x3c, 0x85, 0xe1, 0xda,
    0x96, 0x32, 0x62, 0xbc, 0x3b, 0x5b, 0xd4, 0x8c, 0x30, 0xd5, 0x0f, 0xda,
    0xfb, 0xa9, 0xce, 0xbd, 0x9b, 0xb7, 0x3d, 0x97, 0xd1, 0xd5, 0x54, 0x6d,
    0x4a, 0xd4, 0x62, 0x0d, 0x48, 0x6e, 0x34, 0x73, 0xfb, 0x04, 0x61, 0xaa,
    0x1e, 0x3a, 0xa7, 0x5e, 0xa9, 0xd7, 0x02, 0x30, 0xd2, 0x1e, 0xb3, 0xb4,
    0x89, 0x8d, 0xba, 0xf9, 0xa8, 0x35, 0xbf, 0x84, 0x52, 0x8f, 0x8f, 0xfa,
    0xa1, 0x75, 0x4c, 0x13, 0x0d, 0x61, 0xaa, 0x3f, 0x36, 0xd1, 0x5d, 0x63,
    0x62, 0xa7, 0xed, 0x59, 0x76, 0xbc, 0x48, 0x6c, 0xaa, 0x6f, 0xc4, 0x56,
    0x3e, 0x98, 0xb1, 0x2c, 0xcc, 0xc7, 0x40, 0x06, 0x4e, 0x77, 0x40, 0x91,
    0xd4, 0xe4, 0xe2, 0x74, 0xc7, 0x36, 0x69, 0xb6, 0xde, 0x0b, 0x10, 0x6b,
    0x51, 0x6d, 0x2d, 0x51, 0xa2, 0x16, 0xba, 0xdd, 0x11, 0x4e, 0xb7, 0x8c,
    0x38, 0x03, 0xd7, 0xfc, 0x61, 0xa6, 0x0b, 0x5e, 0xf1, 0xda, 0xbc, 0x1f,
    0xa8, 0x3e, 0x5e, 0x11, 0xbf, 0xec, 0xf1, 0xb1, 0x4b, 0xe7, 0x0c, 0x26,
    0xb1, 0x4d, 0x1c, 0x2d, 0x0b, 0xc4, 0x99, 0x52, 0x0f, 0xa3, 0x98, 0xdd,
    0x9d, 0x0e, 0xd3, 0x59, 0x04, 0xa5, 0xc4, 0x20, 0xe3, 0x6b, 0xb5, 0xd9,
    0x2f, 0x12, 0x17, 0x3c, 0x0f, 0x17, 0xc0, 0x62, 0x5d, 0xb8, 0x52, 0x8f,
    0x75, 0xc2, 0x74, 0x16, 0xd1, 0x6a, 0xc3, 0xe8, 0x5e, 0xa1, 0x9b, 0x42,
    0x9a, 0x3e, 0x1e, 0xdd, 0x4c, 0xd9, 0xdd, 0x39, 0xc2, 0x39, 0x1b, 0x28,
    0x3b, 0x7a, 0x2e, 0x9c, 0x6e, 0x11, 0x1e, 0x57, 0x2d, 0x3b, 0xec, 0xbb,
    0x62, 0x6e, 0x93, 0xb5, 0x7e, 0xb9, 0xb0, 0x80, 0x9f, 0x07, 0x2e, 0x44,
    0xd0, 0x3a, 0x70, 0xd4, 0x0b, 0x73, 0x87, 0xf4, 0x3d, 0xc8, 0x80, 0x7e,
    0x1d, 0xb2, 0x0a, 0xfe, 0x61, 0xf5, 0x19, 0xf9, 0x8a, 0x78, 0x55, 0xa7,
    0xbc, 0x10, 0xb5, 0xe7, 0xa9, 0x6e, 0x8b, 0x38, 0xc9, 0xe1, 0xf0, 0x33,
    0xf9, 0xfd, 0x91, 0x9a, 0xbf, 0xa8, 0x43, 0xbb, 0xc2, 0xe4, 0xc0, 0x26,
    0x57, 0x9e, 0x18, 0xe2, 0x65, 0x21, 0x1a, 0x8f, 0xcd, 0x1d, 0x62, 0x6a,
    0x70, 0x33, 0x00, 0x6a, 0x90, 0x28, 0xaf, 0xb9, 0xa8, 0x43, 0x52, 0x61,
    0xfc, 0x63, 0x4d, 0xfe, 0x9c, 0x78, 0x59, 0x4e, 0x9c, 0x26, 0x2a, 0x4e,
    0xaf, 0x3d, 0x38, 0x5b, 0x8b, 0xa7, 0xc3, 0x1a, 0x1c, 0x56, 0x18, 0xec,
    0x28, 0x71, 0xdb, 0xe3, 0xf7, 0x12, 0xeb, 0xbb, 0xc3, 0x3d, 0x6e, 0x41,
    0x56, 0x83, 0x44, 0x15, 0x94, 0x61, 0x1d, 0xe2, 0x0a, 0xde, 0x76, 0x95,
    0xd7, 0x3d, 0xce, 0x5e, 0x26, 0x8e, 0x35, 0x71, 0x1c, 0xa7, 0xd0, 0x03,
    0xf7, 0x7b, 0x15, 0xda, 0x6b, 0xf1, 0xb8, 0x5f, 0x83, 0xed, 0x2d, 0x6c,
    0x8f, 0xcd, 0x78, 0x27, 0xb9, 0xe9, 0x63, 0xf7, 0x5b, 0x15, 0x92, 0x1a,
    0x44, 0x1e, 0xfc, 0x05, 0xa7, 0xed, 0xb0, 0xe1, 0x07, 0xbe, 0x34, 0xea,
    0x0d, 0xce, 0x7f, 0xff, 0x91, 0x3d, 0xf9, 0x2a, 0xe7, 0x72, 0x29, 0x7b,
    0xd2, 0x94, 0x9e, 0x5c, 0xca, 0xbe, 0x34, 0x9f, 0xe5, 0xbd, 0xc7, 0x4b,
    0xd9, 0xff, 0xa0, 0xe3, 0x63, 0x53, 0x82, 0x5a, 0xb0, 0xd1, 0xa8, 0x37,
    0xe4, 0xe4, 0xcb, 0x47, 0x36, 0x37, 0x45, 0x1e, 0x3e, 0xed, 0xc9, 0xbf,
    0x87, 0x13, 0xf9, 0x0f, 0xb8, 0xb1, 0x49, 0x2a, 0x12, 0x22, 0xc9, 0xaf,
    0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

static const unsigned char img1_setupui[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x16,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xc4, 0xb4, 0x6c, 0x3b, 0x00, 0x00, 0x02,
    0x6e, 0x49, 0x44, 0x41, 0x54, 0x38, 0x8d, 0x8d, 0x94, 0xb1, 0x4b, 0x23,
    0x41, 0x14, 0x87, 0x9f, 0x70, 0x85, 0x65, 0xca, 0x5c, 0x71, 0x60, 0x7b,
    0x65, 0xca, 0xe4, 0x3f, 0x38, 0xae, 0x11, 0xe4, 0x4a, 0x0b, 0xb5, 0xba,
    0xb3, 0x92, 0x70, 0x85, 0x88, 0x45, 0x88, 0x16, 0xb2, 0x55, 0x60, 0xd3,
    0xad, 0x07, 0x81, 0xa4, 0x91, 0xd8, 0x48, 0xce, 0x42, 0x88, 0x85, 0xb0,
    0xdb, 0x25, 0x85, 0xb0, 0x29, 0xb7, 0xb0, 0xd8, 0x83, 0x2d, 0x16, 0x49,
    0xb1, 0x45, 0x10, 0xbe, 0x2b, 0x66, 0x5e, 0xe2, 0xae, 0xba, 0x3a, 0xcd,
    0x9b, 0x37, 0x3b, 0xf3, 0x9b, 0x6f, 0x7e, 0x6f, 0x76, 0x04, 0x10, 0x02,
    0x04, 0x10, 0x22, 0x4c, 0x9b, 0x9a, 0xe0, 0xdd, 0xda, 0x3c, 0x30, 0xc1,
    0x0f, 0x7c, 0x79, 0x3e, 0xbf, 0x2c, 0x5f, 0x26, 0xe1, 0xcc, 0xa8, 0xb6,
    0x6f, 0x53, 0x00, 0x9c, 0x69, 0x06, 0x80, 0x7b, 0x6f, 0x37, 0xb9, 0x8b,
    0x01, 0x18, 0x5d, 0xf9, 0x3c, 0x17, 0x79, 0x2b, 0x1a, 0x51, 0x5d, 0x6c,
    0xc9, 0x46, 0xb3, 0x7c, 0x1c, 0xea, 0x09, 0x34, 0x1f, 0x8c, 0x79, 0x8f,
    0x5c, 0xfc, 0x20, 0x34, 0x93, 0xed, 0xa2, 0x70, 0x6e, 0x63, 0x66, 0x8f,
    0x1f, 0xdb, 0xf8, 0x60, 0x06, 0xfa, 0xb3, 0x14, 0x06, 0x2b, 0x5b, 0xca,
    0x88, 0xf1, 0xee, 0xf2, 0xa2, 0xda, 0xc2, 0xd4, 0x76, 0xac, 0xf7, 0x13,
    0x9b, 0x7b, 0x37, 0xef, 0x7b, 0x2e, 0xc3, 0xab, 0x89, 0x59, 0x94, 0x98,
    0xc9, 0x16, 0x90, 0x4c, 0x35, 0xb3, 0xfc, 0x09, 0xc2, 0xd4, 0x74, 0xda,
    0xa7, 0x5e, 0xa9, 0xd7, 0x02, 0x30, 0xb4, 0x1e, 0xb3, 0xc8, 0x13, 0xab,
    0xba, 0x6e, 0x5a, 0x1c, 0x2f, 0xf5, 0xf8, 0xa8, 0x17, 0xe6, 0x8e, 0xa9,
    0x51, 0x09, 0x53, 0xbb, 0xd9, 0xd8, 0xde, 0x1a, 0x8d, 0xed, 0x96, 0x97,
    0xb3, 0xe3, 0x55, 0x62, 0xad, 0xbe, 0x8a, 0x2d, 0x89, 0xb4, 0x2d, 0x0a,
    0xe3, 0xb1, 0xe9, 0x3b, 0x9d, 0x3e, 0x45, 0x52, 0xcd, 0xc5, 0xe9, 0x8c,
    0xf2, 0xa4, 0xf3, 0xd5, 0x5a, 0x80, 0xd8, 0x8a, 0x5a, 0x6b, 0x89, 0x12,
    0x33, 0xd1, 0xed, 0x0c, 0x71, 0x3a, 0x65, 0xc4, 0x19, 0xb8, 0xfa, 0x87,
    0xe9, 0x2d, 0x78, 0xc3, 0x6b, 0xfd, 0xde, 0x37, 0xf7, 0x78, 0x49, 0xfc,
    0xba, 0xc7, 0xc7, 0x2e, 0xed, 0x33, 0x18, 0xc7, 0x79, 0xe2, 0x68, 0x51,
    0x20, 0x9e, 0x1b, 0xf5, 0x30, 0x8a, 0xd9, 0xdd, 0x69, 0x33, 0x99, 0x46,
    0x50, 0x4a, 0x0c, 0x32, 0xba, 0x36, 0x8b, 0xfd, 0x22, 0x71, 0xc1, 0xf3,
    0xf0, 0xc1, 0x78, 0xa4, 0x85, 0x2b, 0xf5, 0xd8, 0x26, 0x4c, 0xa6, 0x11,
    0xcd, 0x16, 0x0c, 0xef, 0x0d, 0xba, 0x16, 0x52, 0xef, 0xf1, 0xf0, 0x66,
    0xc2, 0xee, 0xce, 0x11, 0xce, 0x59, 0xdf, 0xd8, 0xd1, 0x75, 0xe1, 0x74,
    0x83, 0xf0, 0xb8, 0x9a, 0xb3, 0x23, 0xff, 0x56, 0xcc, 0xf2, 0x64, 0xcd,
    0xdf, 0x2e, 0xc4, 0xf0, 0xeb, 0xc0, 0x85, 0x07, 0x68, 0x1e, 0x38, 0xe6,
    0x83, 0xbe, 0x21, 0x3d, 0xcf, 0xcc, 0xed, 0xd5, 0x21, 0xab, 0xe0, 0x1f,
    0x56, 0x5f, 0x90, 0x2f, 0x89, 0x97, 0x75, 0xca, 0x0a, 0xd1, 0x7a, 0x9e,
    0xda, 0x6b, 0x11, 0x27, 0x19, 0x1c, 0x7e, 0x25, 0xbb, 0x3f, 0x32, 0xe3,
    0x17, 0x75, 0x68, 0x55, 0x18, 0x1f, 0xe4, 0xc9, 0x8d, 0x27, 0x4a, 0xbc,
    0x28, 0x44, 0xf5, 0x58, 0xdf, 0x10, 0xad, 0xc1, 0x4d, 0x1f, 0xa8, 0x41,
    0x62, 0xbc, 0xe6, 0xa2, 0x0e, 0x69, 0x85, 0xd1, 0xcf, 0x15, 0xf9, 0x4b,
    0xe2, 0x45, 0x39, 0x71, 0x9a, 0x98, 0x38, 0xb9, 0xf6, 0xe0, 0x6c, 0x25,
    0x9e, 0x0e, 0x6a, 0x70, 0x58, 0xa1, 0xbf, 0x63, 0xc4, 0xf3, 0x1e, 0x7f,
    0x94, 0xd8, 0xbe, 0x1d, 0xee, 0x71, 0x13, 0xb2, 0x1a, 0x24, 0xa6, 0xa0,
    0x0c, 0xea, 0x90, 0x54, 0xf0, 0xb6, 0xab, 0xbc, 0xed, 0xf1, 0xfc, 0x75,
    0xe2, 0xd8, 0x12, 0xc7, 0x71, 0x0a, 0x5d, 0x70, 0x7f, 0x54, 0xa1, 0xb5,
    0x12, 0x8f, 0x7b, 0x35, 0xd8, 0xde, 0x20, 0xef, 0xb1, 0xb6, 0x0f, 0x92,
    0xeb, 0x3d, 0x76, 0x37, 0xab, 0x90, 0xd6, 0x20, 0xf2, 0x20, 0x02, 0xa7,
    0xe5, 0xb0, 0xe6, 0x07, 0xbe, 0x34, 0xea, 0x0d, 0xce, 0xff, 0xfc, 0x95,
    0x3d, 0xf9, 0x2e, 0xe7, 0x72, 0x29, 0x7b, 0xb2, 0x25, 0x5d, 0xb9, 0x94,
    0x7d, 0xd9, 0x7a, 0x91, 0x77, 0x9f, 0x2e, 0x65, 0xff, 0x93, 0x8d, 0x4f,
    0x5b, 0x12, 0xd4, 0x82, 0xb5, 0x46, 0xbd, 0x21, 0x27, 0xdf, 0x3e, 0xb3,
    0xbe, 0x2e, 0xf2, 0xf8, 0x65, 0x4f, 0xfe, 0x3d, 0x9e, 0xc8, 0x7f, 0x40,
    0x86, 0x49, 0x5f, 0xbe, 0xd9, 0x30, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x49,
    0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};


/*
 *  Constructs a setupui as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
setupui::setupui( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    QImage img;
    img.loadFromData( img0_setupui, sizeof( img0_setupui ), "PNG" );
    image0 = img;
    img.loadFromData( img1_setupui, sizeof( img1_setupui ), "PNG" );
    image1 = img;
    if ( !name )
	setName( "setupui" );
    setSizeGripEnabled( TRUE );
    setupuiLayout = new QGridLayout( this, 1, 1, 11, 6, "setupuiLayout"); 

    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setAutoDefault( TRUE );
    Layout1->addWidget( buttonCancel );

    setupuiLayout->addLayout( Layout1, 1, 0 );

    tabWidget2 = new QTabWidget( this, "tabWidget2" );
    tabWidget2->setFocusPolicy( QTabWidget::NoFocus );

    tabPersonal = new QWidget( tabWidget2, "tabPersonal" );
    tabPersonalLayout = new QGridLayout( tabPersonal, 1, 1, 11, 6, "tabPersonalLayout"); 

    textLabel1 = new QLabel( tabPersonal, "textLabel1" );
    textLabel1->setFrameShape( QLabel::LineEditPanel );

    tabPersonalLayout->addMultiCellWidget( textLabel1, 0, 0, 0, 1 );

    MyLocatorkLineEdit = new KLineEdit( tabPersonal, "MyLocatorkLineEdit" );

    tabPersonalLayout->addMultiCellWidget( MyLocatorkLineEdit, 1, 1, 1, 2 );

    textLabel2 = new QLabel( tabPersonal, "textLabel2" );
    textLabel2->setFrameShape( QLabel::LineEditPanel );

    tabPersonalLayout->addWidget( textLabel2, 1, 0 );

    MyCallkLineEdit = new KLineEdit( tabPersonal, "MyCallkLineEdit" );

    tabPersonalLayout->addWidget( MyCallkLineEdit, 0, 2 );
    spacer3 = new QSpacerItem( 120, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabPersonalLayout->addItem( spacer3, 1, 3 );
    spacer2 = new QSpacerItem( 120, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabPersonalLayout->addItem( spacer2, 0, 3 );
    tabWidget2->insertTab( tabPersonal, QString::fromLatin1("") );

    tab = new QWidget( tabWidget2, "tab" );
    tabLayout = new QGridLayout( tab, 1, 1, 11, 6, "tabLayout"); 

    frame3 = new QFrame( tab, "frame3" );
    frame3->setFrameShape( QFrame::StyledPanel );
    frame3->setFrameShadow( QFrame::Raised );
    frame3Layout = new QGridLayout( frame3, 1, 1, 11, 6, "frame3Layout"); 

    layout5 = new QVBoxLayout( 0, 0, 6, "layout5"); 

    lastFilecheckBox = new QCheckBox( frame3, "lastFilecheckBox" );
    QFont lastFilecheckBox_font(  lastFilecheckBox->font() );
    lastFilecheckBox_font.setBold( TRUE );
    lastFilecheckBox->setFont( lastFilecheckBox_font ); 
    layout5->addWidget( lastFilecheckBox );

    layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 

    filenamekLineEdit = new KLineEdit( frame3, "filenamekLineEdit" );
    filenamekLineEdit->setEnabled( FALSE );
    layout4->addWidget( filenamekLineEdit );

    searchFilekPushButton = new KPushButton( frame3, "searchFilekPushButton" );
    searchFilekPushButton->setEnabled( FALSE );
    searchFilekPushButton->setPixmap( image0 );
    layout4->addWidget( searchFilekPushButton );
    layout5->addLayout( layout4 );

    frame3Layout->addMultiCellLayout( layout5, 0, 0, 0, 1 );

    checkBoxProgressDialog = new QCheckBox( frame3, "checkBoxProgressDialog" );
    QFont checkBoxProgressDialog_font(  checkBoxProgressDialog->font() );
    checkBoxProgressDialog_font.setBold( TRUE );
    checkBoxProgressDialog->setFont( checkBoxProgressDialog_font ); 
    checkBoxProgressDialog->setChecked( TRUE );

    frame3Layout->addMultiCellWidget( checkBoxProgressDialog, 1, 1, 0, 1 );

    UTCcheckBox = new QCheckBox( frame3, "UTCcheckBox" );
    QFont UTCcheckBox_font(  UTCcheckBox->font() );
    UTCcheckBox_font.setBold( TRUE );
    UTCcheckBox->setFont( UTCcheckBox_font ); 
    UTCcheckBox->setFocusPolicy( QCheckBox::NoFocus );
    UTCcheckBox->setChecked( TRUE );
    UTCcheckBox->setTristate( FALSE );

    frame3Layout->addWidget( UTCcheckBox, 2, 0 );

    realTimeLogCheckBox = new QCheckBox( frame3, "realTimeLogCheckBox" );
    QFont realTimeLogCheckBox_font(  realTimeLogCheckBox->font() );
    realTimeLogCheckBox_font.setBold( TRUE );
    realTimeLogCheckBox->setFont( realTimeLogCheckBox_font ); 
    realTimeLogCheckBox->setChecked( TRUE );

    frame3Layout->addWidget( realTimeLogCheckBox, 2, 1 );

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 

    textLabel1_4 = new QLabel( frame3, "textLabel1_4" );
    textLabel1_4->setPaletteBackgroundColor( QColor( 238, 238, 230 ) );
    textLabel1_4->setFrameShape( QLabel::LineEditPanel );
    layout2->addWidget( textLabel1_4 );

    powerkIntNumInput = new KIntNumInput( frame3, "powerkIntNumInput" );
    powerkIntNumInput->setValue( 90 );
    layout2->addWidget( powerkIntNumInput );

    frame3Layout->addMultiCellLayout( layout2, 3, 3, 0, 1 );

    textLabel1_5 = new QLabel( frame3, "textLabel1_5" );
    textLabel1_5->setFrameShape( QLabel::LineEditPanel );

    frame3Layout->addWidget( textLabel1_5, 4, 0 );

    bandkComboBox = new KComboBox( FALSE, frame3, "bandkComboBox" );

    frame3Layout->addWidget( bandkComboBox, 4, 1 );

    modekComboBox = new KComboBox( FALSE, frame3, "modekComboBox" );

    frame3Layout->addWidget( modekComboBox, 5, 1 );

    textLabel2_4 = new QLabel( frame3, "textLabel2_4" );
    textLabel2_4->setFrameShape( QLabel::LineEditPanel );

    frame3Layout->addWidget( textLabel2_4, 5, 0 );

    tabLayout->addWidget( frame3, 0, 0 );
    tabWidget2->insertTab( tab, QString::fromLatin1("") );

    tabCluster = new QWidget( tabWidget2, "tabCluster" );
    tabClusterLayout = new QGridLayout( tabCluster, 1, 1, 11, 6, "tabClusterLayout"); 

    layout10 = new QHBoxLayout( 0, 0, 6, "layout10"); 

    groupBox3 = new QGroupBox( tabCluster, "groupBox3" );
    QFont groupBox3_font(  groupBox3->font() );
    groupBox3_font.setBold( TRUE );
    groupBox3->setFont( groupBox3_font ); 
    groupBox3->setFrameShape( QGroupBox::LineEditPanel );
    groupBox3->setFrameShadow( QGroupBox::Raised );
    groupBox3->setColumnLayout(0, Qt::Vertical );
    groupBox3->layout()->setSpacing( 6 );
    groupBox3->layout()->setMargin( 11 );
    groupBox3Layout = new QGridLayout( groupBox3->layout() );
    groupBox3Layout->setAlignment( Qt::AlignTop );

    HFcheckBox = new QCheckBox( groupBox3, "HFcheckBox" );
    QFont HFcheckBox_font(  HFcheckBox->font() );
    HFcheckBox->setFont( HFcheckBox_font ); 
    HFcheckBox->setChecked( TRUE );

    groupBox3Layout->addWidget( HFcheckBox, 0, 0 );

    VHFcheckBox = new QCheckBox( groupBox3, "VHFcheckBox" );
    QFont VHFcheckBox_font(  VHFcheckBox->font() );
    VHFcheckBox->setFont( VHFcheckBox_font ); 
    VHFcheckBox->setChecked( TRUE );

    groupBox3Layout->addWidget( VHFcheckBox, 1, 0 );

    WARCcheckBox = new QCheckBox( groupBox3, "WARCcheckBox" );
    QFont WARCcheckBox_font(  WARCcheckBox->font() );
    WARCcheckBox->setFont( WARCcheckBox_font ); 
    WARCcheckBox->setChecked( TRUE );

    groupBox3Layout->addWidget( WARCcheckBox, 2, 0 );

    confirmedcheckBox = new QCheckBox( groupBox3, "confirmedcheckBox" );
    QFont confirmedcheckBox_font(  confirmedcheckBox->font() );
    confirmedcheckBox->setFont( confirmedcheckBox_font ); 
    confirmedcheckBox->setChecked( TRUE );

    groupBox3Layout->addWidget( confirmedcheckBox, 3, 0 );
    layout10->addWidget( groupBox3 );

    groupBox2 = new QGroupBox( tabCluster, "groupBox2" );
    QFont groupBox2_font(  groupBox2->font() );
    groupBox2_font.setBold( TRUE );
    groupBox2->setFont( groupBox2_font ); 
    groupBox2->setFrameShape( QGroupBox::LineEditPanel );
    groupBox2->setFrameShadow( QGroupBox::Raised );
    groupBox2->setColumnLayout(0, Qt::Vertical );
    groupBox2->layout()->setSpacing( 6 );
    groupBox2->layout()->setMargin( 11 );
    groupBox2Layout = new QGridLayout( groupBox2->layout() );
    groupBox2Layout->setAlignment( Qt::AlignTop );

    ANNcheckBox = new QCheckBox( groupBox2, "ANNcheckBox" );
    QFont ANNcheckBox_font(  ANNcheckBox->font() );
    ANNcheckBox->setFont( ANNcheckBox_font ); 
    ANNcheckBox->setChecked( TRUE );

    groupBox2Layout->addWidget( ANNcheckBox, 0, 0 );

    WCYcheckBox = new QCheckBox( groupBox2, "WCYcheckBox" );
    QFont WCYcheckBox_font(  WCYcheckBox->font() );
    WCYcheckBox->setFont( WCYcheckBox_font ); 
    WCYcheckBox->setChecked( TRUE );

    groupBox2Layout->addWidget( WCYcheckBox, 2, 0 );

    WXcheckBox = new QCheckBox( groupBox2, "WXcheckBox" );
    QFont WXcheckBox_font(  WXcheckBox->font() );
    WXcheckBox->setFont( WXcheckBox_font ); 
    WXcheckBox->setChecked( TRUE );

    groupBox2Layout->addWidget( WXcheckBox, 1, 0 );
    layout10->addWidget( groupBox2 );

    tabClusterLayout->addLayout( layout10, 2, 0 );

    layout13 = new QHBoxLayout( 0, 0, 6, "layout13"); 
    spacer8_2 = new QSpacerItem( 329, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout13->addItem( spacer8_2 );

    addDxClusterpushButton = new QPushButton( tabCluster, "addDxClusterpushButton" );
    layout13->addWidget( addDxClusterpushButton );

    clearDxClusterpushButton = new QPushButton( tabCluster, "clearDxClusterpushButton" );
    layout13->addWidget( clearDxClusterpushButton );

    tabClusterLayout->addLayout( layout13, 1, 0 );

    dxClustercomboBox = new QComboBox( FALSE, tabCluster, "dxClustercomboBox" );

    tabClusterLayout->addWidget( dxClustercomboBox, 0, 0 );
    tabWidget2->insertTab( tabCluster, QString::fromLatin1("") );

    tab_2 = new QWidget( tabWidget2, "tab_2" );
    tabLayout_2 = new QGridLayout( tab_2, 1, 1, 11, 6, "tabLayout_2"); 

    textLabel1_3 = new QLabel( tab_2, "textLabel1_3" );
    textLabel1_3->setFrameShape( QLabel::LineEditPanel );

    tabLayout_2->addWidget( textLabel1_3, 0, 0 );
    spacer9 = new QSpacerItem( 140, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout_2->addItem( spacer9, 1, 2 );

    textLabel2_3 = new QLabel( tab_2, "textLabel2_3" );
    textLabel2_3->setFrameShape( QLabel::LineEditPanel );

    tabLayout_2->addWidget( textLabel2_3, 1, 0 );
    spacer10 = new QSpacerItem( 140, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout_2->addItem( spacer10, 2, 2 );
    spacer10_2 = new QSpacerItem( 248, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout_2->addItem( spacer10_2, 3, 2 );

    defaulttextLabel = new QLabel( tab_2, "defaulttextLabel" );
    defaulttextLabel->setFrameShape( QLabel::LineEditPanel );

    tabLayout_2->addWidget( defaulttextLabel, 3, 0 );

    textLabel3 = new QLabel( tab_2, "textLabel3" );
    textLabel3->setFrameShape( QLabel::LineEditPanel );

    tabLayout_2->addWidget( textLabel3, 2, 0 );

    confirmedkColorCombo = new KColorCombo( tab_2, "confirmedkColorCombo" );

    tabLayout_2->addWidget( confirmedkColorCombo, 0, 1 );

    workedkColorCombo = new KColorCombo( tab_2, "workedkColorCombo" );

    tabLayout_2->addWidget( workedkColorCombo, 1, 1 );

    neededkColorCombo = new KColorCombo( tab_2, "neededkColorCombo" );

    tabLayout_2->addWidget( neededkColorCombo, 2, 1 );

    defaultkColorCombo = new KColorCombo( tab_2, "defaultkColorCombo" );
    defaultkColorCombo->setColor( QColor( 128, 128, 128 ) );

    tabLayout_2->addWidget( defaultkColorCombo, 3, 1 );
    spacer8 = new QSpacerItem( 140, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout_2->addItem( spacer8, 0, 2 );
    tabWidget2->insertTab( tab_2, QString::fromLatin1("") );

    TabPage = new QWidget( tabWidget2, "TabPage" );
    TabPageLayout = new QGridLayout( TabPage, 1, 1, 11, 6, "TabPageLayout"); 

    completeWithPreviouscheckBox = new QCheckBox( TabPage, "completeWithPreviouscheckBox" );
    completeWithPreviouscheckBox->setEnabled( TRUE );
    QFont completeWithPreviouscheckBox_font(  completeWithPreviouscheckBox->font() );
    completeWithPreviouscheckBox_font.setBold( TRUE );
    completeWithPreviouscheckBox->setFont( completeWithPreviouscheckBox_font ); 
    completeWithPreviouscheckBox->setChecked( FALSE );

    TabPageLayout->addWidget( completeWithPreviouscheckBox, 0, 0 );

    mandatoryDatacheckBox = new QCheckBox( TabPage, "mandatoryDatacheckBox" );
    mandatoryDatacheckBox->setEnabled( TRUE );
    QFont mandatoryDatacheckBox_font(  mandatoryDatacheckBox->font() );
    mandatoryDatacheckBox_font.setBold( TRUE );
    mandatoryDatacheckBox->setFont( mandatoryDatacheckBox_font ); 
    mandatoryDatacheckBox->setChecked( FALSE );

    TabPageLayout->addWidget( mandatoryDatacheckBox, 1, 0 );
    tabWidget2->insertTab( TabPage, QString::fromLatin1("") );

    TabPage_2 = new QWidget( tabWidget2, "TabPage_2" );
    TabPageLayout_2 = new QGridLayout( TabPage_2, 1, 1, 11, 6, "TabPageLayout_2"); 

    layout6 = new QHBoxLayout( 0, 0, 6, "layout6"); 

    hamlibcheckBox = new QCheckBox( TabPage_2, "hamlibcheckBox" );
    layout6->addWidget( hamlibcheckBox );

    layout5_2 = new QHBoxLayout( 0, 0, 6, "layout5_2"); 

    hamlibIntervalspinBox = new QSpinBox( TabPage_2, "hamlibIntervalspinBox" );
    hamlibIntervalspinBox->setMaxValue( 5000 );
    hamlibIntervalspinBox->setValue( 200 );
    layout5_2->addWidget( hamlibIntervalspinBox );

    textLabel1_6 = new QLabel( TabPage_2, "textLabel1_6" );
    layout5_2->addWidget( textLabel1_6 );
    layout6->addLayout( layout5_2 );

    TabPageLayout_2->addMultiCellLayout( layout6, 0, 0, 0, 1 );

    layout10_2 = new QHBoxLayout( 0, 0, 6, "layout10_2"); 

    textLabel1_7 = new QLabel( TabPage_2, "textLabel1_7" );
    layout10_2->addWidget( textLabel1_7 );

    devicelineEdit = new QLineEdit( TabPage_2, "devicelineEdit" );
    devicelineEdit->setMaxLength( 50 );
    layout10_2->addWidget( devicelineEdit );

    TabPageLayout_2->addLayout( layout10_2, 2, 0 );

    layout7 = new QHBoxLayout( 0, 0, 6, "layout7"); 

    comboBoxTrvSpeedList = new QComboBox( FALSE, TabPage_2, "comboBoxTrvSpeedList" );
    layout7->addWidget( comboBoxTrvSpeedList );

    TabPageLayout_2->addLayout( layout7, 2, 1 );

    layout14 = new QHBoxLayout( 0, 0, 6, "layout14"); 

    textLabel1_8 = new QLabel( TabPage_2, "textLabel1_8" );
    textLabel1_8->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, textLabel1_8->sizePolicy().hasHeightForWidth() ) );
    layout14->addWidget( textLabel1_8 );

    comboBoxTrvList = new QComboBox( FALSE, TabPage_2, "comboBoxTrvList" );
    layout14->addWidget( comboBoxTrvList );

    TabPageLayout_2->addLayout( layout14, 1, 0 );
    tabWidget2->insertTab( TabPage_2, QString::fromLatin1("") );

    TabPage_3 = new QWidget( tabWidget2, "TabPage_3" );
    TabPageLayout_3 = new QGridLayout( TabPage_3, 1, 1, 11, 6, "TabPageLayout_3"); 

    awardFilekPushButton = new KPushButton( TabPage_3, "awardFilekPushButton" );
    awardFilekPushButton->setEnabled( TRUE );
    awardFilekPushButton->setPixmap( image1 );

    TabPageLayout_3->addWidget( awardFilekPushButton, 0, 4 );

    awardRemovepushButton = new QPushButton( TabPage_3, "awardRemovepushButton" );

    TabPageLayout_3->addMultiCellWidget( awardRemovepushButton, 1, 1, 3, 4 );

    awardFilenamekLineEdit = new KLineEdit( TabPage_3, "awardFilenamekLineEdit" );
    awardFilenamekLineEdit->setEnabled( TRUE );

    TabPageLayout_3->addMultiCellWidget( awardFilenamekLineEdit, 0, 0, 0, 3 );
    spacer4_2 = new QSpacerItem( 211, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    TabPageLayout_3->addItem( spacer4_2, 1, 2 );

    awardClearpushButton = new QPushButton( TabPage_3, "awardClearpushButton" );

    TabPageLayout_3->addWidget( awardClearpushButton, 1, 1 );

    awardAddpushButton = new QPushButton( TabPage_3, "awardAddpushButton" );

    TabPageLayout_3->addWidget( awardAddpushButton, 1, 0 );

    awardListView = new QListView( TabPage_3, "awardListView" );
    awardListView->addColumn( tr2i18n( "Number" ) );
    awardListView->addColumn( tr2i18n( "Award" ) );
    awardListView->addColumn( tr2i18n( "Entity" ) );
    awardListView->addColumn( tr2i18n( "File" ) );

    TabPageLayout_3->addMultiCellWidget( awardListView, 2, 2, 0, 4 );
    tabWidget2->insertTab( TabPage_3, QString::fromLatin1("") );

    setupuiLayout->addWidget( tabWidget2, 0, 0 );
    languageChange();
    resize( QSize(638, 449).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( slotOkBtnClicked() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( searchFilekPushButton, SIGNAL( clicked() ), this, SLOT( slotFileNameClicked() ) );
    connect( lastFilecheckBox, SIGNAL( clicked() ), this, SLOT( slotOpenFileButtonClicked() ) );
    connect( awardFilekPushButton, SIGNAL( clicked() ), this, SLOT( slotSearchAwardFilekPushButtonClicked() ) );
    connect( awardAddpushButton, SIGNAL( clicked() ), this, SLOT( slotAddButtonClicked() ) );
    connect( awardClearpushButton, SIGNAL( clicked() ), this, SLOT( slotAwardClearButtonClicked() ) );
    connect( awardRemovepushButton, SIGNAL( clicked() ), this, SLOT( slotAwardRemoveButtonClicked() ) );
    connect( addDxClusterpushButton, SIGNAL( clicked() ), this, SLOT( slotAddDxClusterPushButtonClicked() ) );
    connect( clearDxClusterpushButton, SIGNAL( clicked() ), this, SLOT( slotClearDxClusterPushButtonClicked() ) );
    connect( awardListView, SIGNAL( rightButtonClicked(QListViewItem*,const QPoint&,int) ), this, SLOT( slotAwardRightButtonFromList(QListViewItem*,const QPoint&) ) );
    connect( awardListView, SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT( slotAwardSelected(QListViewItem*) ) );

    // tab order
    setTabOrder( MyCallkLineEdit, MyLocatorkLineEdit );
    setTabOrder( MyLocatorkLineEdit, HFcheckBox );
    setTabOrder( HFcheckBox, VHFcheckBox );
    setTabOrder( VHFcheckBox, ANNcheckBox );
    setTabOrder( ANNcheckBox, WXcheckBox );
    setTabOrder( WXcheckBox, buttonOk );
    setTabOrder( buttonOk, buttonCancel );
    setTabOrder( buttonCancel, tabWidget2 );
}

/*
 *  Destroys the object and frees any allocated resources
 */
setupui::~setupui()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void setupui::languageChange()
{
    setCaption( tr2i18n( "KLog setup" ) );
    buttonOk->setText( tr2i18n( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr2i18n( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
    textLabel1->setText( tr2i18n( "<b>QRZ:</b>" ) );
    QToolTip::add( MyLocatorkLineEdit, tr2i18n( "Enter your locator - IN80EK" ) );
    textLabel2->setText( tr2i18n( "<b>Locator:</b>" ) );
    QToolTip::add( MyCallkLineEdit, tr2i18n( "Enter your QRZ - EA4TV" ) );
    tabWidget2->changeTab( tabPersonal, tr2i18n( "Personal" ) );
    lastFilecheckBox->setText( tr2i18n( "Open this file when starting:" ) );
    searchFilekPushButton->setText( QString::null );
    checkBoxProgressDialog->setText( tr2i18n( "Show progress dialog when reading log file" ) );
    QToolTip::add( checkBoxProgressDialog, tr2i18n( "Shows a progress bar but makes the log reading slower" ) );
    UTCcheckBox->setText( tr2i18n( "Time in UTC" ) );
    QToolTip::add( UTCcheckBox, tr2i18n( "If checked the time will be UTC" ) );
    realTimeLogCheckBox->setText( tr2i18n( "Logging in real time" ) );
    textLabel1_4->setText( tr2i18n( "<b>Power:</b>" ) );
    textLabel1_5->setText( tr2i18n( "<b>Band:</b>" ) );
    bandkComboBox->clear();
    bandkComboBox->insertItem( tr2i18n( "160M" ) );
    bandkComboBox->insertItem( tr2i18n( "80M" ) );
    bandkComboBox->insertItem( tr2i18n( "60M" ) );
    bandkComboBox->insertItem( tr2i18n( "40M" ) );
    bandkComboBox->insertItem( tr2i18n( "30M" ) );
    bandkComboBox->insertItem( tr2i18n( "20M" ) );
    bandkComboBox->insertItem( tr2i18n( "17M" ) );
    bandkComboBox->insertItem( tr2i18n( "15M" ) );
    bandkComboBox->insertItem( tr2i18n( "12M" ) );
    bandkComboBox->insertItem( tr2i18n( "10M" ) );
    bandkComboBox->insertItem( tr2i18n( "6M" ) );
    bandkComboBox->insertItem( tr2i18n( "2M" ) );
    bandkComboBox->insertItem( tr2i18n( "125CM" ) );
    bandkComboBox->insertItem( tr2i18n( "70CM" ) );
    bandkComboBox->insertItem( tr2i18n( "33CM" ) );
    bandkComboBox->insertItem( tr2i18n( "23CM" ) );
    bandkComboBox->insertItem( tr2i18n( "13CM" ) );
    modekComboBox->clear();
    modekComboBox->insertItem( tr2i18n( "SSB" ) );
    modekComboBox->insertItem( tr2i18n( "CW" ) );
    modekComboBox->insertItem( tr2i18n( "FM" ) );
    modekComboBox->insertItem( tr2i18n( "PSK31" ) );
    modekComboBox->insertItem( tr2i18n( "SSTV" ) );
    modekComboBox->insertItem( tr2i18n( "RTTY" ) );
    modekComboBox->insertItem( tr2i18n( "MTTY" ) );
    modekComboBox->insertItem( tr2i18n( "PKT" ) );
    modekComboBox->insertItem( tr2i18n( "AM" ) );
    modekComboBox->insertItem( tr2i18n( "FAX" ) );
    modekComboBox->insertItem( tr2i18n( "HELL" ) );
    modekComboBox->insertItem( tr2i18n( "JT44" ) );
    modekComboBox->insertItem( tr2i18n( "MT63" ) );
    modekComboBox->insertItem( tr2i18n( "CLO" ) );
    modekComboBox->insertItem( tr2i18n( "TOR" ) );
    modekComboBox->insertItem( tr2i18n( "FSK" ) );
    modekComboBox->insertItem( tr2i18n( "MFSK8" ) );
    modekComboBox->insertItem( tr2i18n( "MFSK16" ) );
    modekComboBox->insertItem( tr2i18n( "HSFK" ) );
    modekComboBox->insertItem( tr2i18n( "ATV" ) );
    modekComboBox->insertItem( tr2i18n( "PAC" ) );
    modekComboBox->insertItem( tr2i18n( "PAC2" ) );
    modekComboBox->insertItem( tr2i18n( "PAC3" ) );
    modekComboBox->insertItem( tr2i18n( "PSK63" ) );
    modekComboBox->insertItem( tr2i18n( "PSK125" ) );
    modekComboBox->insertItem( tr2i18n( "Q15" ) );
    modekComboBox->insertItem( tr2i18n( "THRB" ) );
    modekComboBox->insertItem( tr2i18n( "ASCI" ) );
    textLabel2_4->setText( tr2i18n( "<b>Mode:</b>" ) );
    tabWidget2->changeTab( tab, tr2i18n( "Misc" ) );
    groupBox3->setTitle( tr2i18n( "Spots" ) );
    HFcheckBox->setText( tr2i18n( "Show HF activity" ) );
    QToolTip::add( HFcheckBox, tr2i18n( "Show 1.8-30MHz spots" ) );
    VHFcheckBox->setText( tr2i18n( "Show V/UHF activity" ) );
    QToolTip::add( VHFcheckBox, tr2i18n( "Show from 50MHz to GHz spots" ) );
    WARCcheckBox->setText( tr2i18n( "Show WARC activity" ) );
    QToolTip::add( WARCcheckBox, tr2i18n( "Show from 50MHz to GHz spots" ) );
    confirmedcheckBox->setText( tr2i18n( "Show confirmed" ) );
    QToolTip::add( confirmedcheckBox, tr2i18n( "Show from 50MHz to GHz spots" ) );
    groupBox2->setTitle( tr2i18n( "Messages" ) );
    ANNcheckBox->setText( tr2i18n( "Show ANN/FULL messages" ) );
    QToolTip::add( ANNcheckBox, tr2i18n( "Show announces" ) );
    WCYcheckBox->setText( tr2i18n( "Show WCY messages" ) );
    QToolTip::add( WCYcheckBox, tr2i18n( "Show weather messages" ) );
    WXcheckBox->setText( tr2i18n( "Show WWV messages" ) );
    QToolTip::add( WXcheckBox, tr2i18n( "Show weather messages" ) );
    addDxClusterpushButton->setText( tr2i18n( "Add" ) );
    QToolTip::add( addDxClusterpushButton, tr2i18n( "Add the DX-Cluster server to the list" ) );
    clearDxClusterpushButton->setText( tr2i18n( "Remove" ) );
    QToolTip::add( clearDxClusterpushButton, tr2i18n( "Clears the text line but also will remove the server from the list if is already added" ) );
    QToolTip::add( dxClustercomboBox, tr2i18n( "Select a dx-cluster server." ) );
    tabWidget2->changeTab( tabCluster, tr2i18n( "DX-Cluster" ) );
    textLabel1_3->setText( tr2i18n( "<b>Confirmed</b>" ) );
    textLabel2_3->setText( tr2i18n( "<b>Worked</b>" ) );
    defaulttextLabel->setText( tr2i18n( "<b>Default</b>" ) );
    textLabel3->setText( tr2i18n( "<b>Needed</b>" ) );
    confirmedkColorCombo->setCurrentItem( 1 );
    workedkColorCombo->setCurrentItem( 8 );
    neededkColorCombo->setCurrentItem( 2 );
    defaultkColorCombo->setCurrentItem( 16 );
    tabWidget2->changeTab( tab_2, tr2i18n( "Colors" ) );
    completeWithPreviouscheckBox->setText( tr2i18n( "Complete data with previous QSOs" ) );
    mandatoryDatacheckBox->setText( tr2i18n( "Requires the mandatory fields in ALL QSO" ) );
    QToolTip::add( mandatoryDatacheckBox, tr2i18n( "Mandatory data is: QRZ, date, time, band, mode, RST(rx) & RST (tx)" ) );
    tabWidget2->changeTab( TabPage, tr2i18n( "QSO" ) );
    hamlibcheckBox->setText( tr2i18n( "Activate Hamlib" ) );
    textLabel1_6->setText( tr2i18n( "Pool hamlib each (msecs)" ) );
    textLabel1_7->setText( tr2i18n( "Device:" ) );
    devicelineEdit->setText( tr2i18n( "/dev/ttyS0" ) );
    comboBoxTrvSpeedList->clear();
    comboBoxTrvSpeedList->insertItem( tr2i18n( "1200" ) );
    comboBoxTrvSpeedList->insertItem( tr2i18n( "2400" ) );
    comboBoxTrvSpeedList->insertItem( tr2i18n( "4800" ) );
    comboBoxTrvSpeedList->insertItem( tr2i18n( "9600" ) );
    comboBoxTrvSpeedList->insertItem( tr2i18n( "19200" ) );
    comboBoxTrvSpeedList->insertItem( tr2i18n( "38400" ) );
    comboBoxTrvSpeedList->insertItem( tr2i18n( "57600" ) );
    comboBoxTrvSpeedList->insertItem( tr2i18n( "115200" ) );
    textLabel1_8->setText( tr2i18n( "Transceiver:" ) );
    tabWidget2->changeTab( TabPage_2, tr2i18n( "Hamlib" ) );
    awardFilekPushButton->setText( QString::null );
    awardRemovepushButton->setText( tr2i18n( "Remove" ) );
    awardClearpushButton->setText( tr2i18n( "Clear" ) );
    awardAddpushButton->setText( tr2i18n( "Add" ) );
    awardListView->header()->setLabel( 0, tr2i18n( "Number" ) );
    awardListView->header()->setLabel( 1, tr2i18n( "Award" ) );
    awardListView->header()->setLabel( 2, tr2i18n( "Entity" ) );
    awardListView->header()->setLabel( 3, tr2i18n( "File" ) );
    tabWidget2->changeTab( TabPage_3, tr2i18n( "Awards" ) );
}

void setupui::slotOkBtnClicked()
{
    qWarning( "setupui::slotOkBtnClicked(): Not implemented yet" );
}

void setupui::slotCancelBtnClicked()
{
    qWarning( "setupui::slotCancelBtnClicked(): Not implemented yet" );
}

void setupui::slotFileNameClicked()
{
    qWarning( "setupui::slotFileNameClicked(): Not implemented yet" );
}

void setupui::slotOpenFileButtonClicked()
{
    qWarning( "setupui::slotOpenFileButtonClicked(): Not implemented yet" );
}

void setupui::slotSearchAwardFilekPushButtonClicked()
{
    qWarning( "setupui::slotSearchAwardFilekPushButtonClicked(): Not implemented yet" );
}

void setupui::slotAddButtonClicked()
{
    qWarning( "setupui::slotAddButtonClicked(): Not implemented yet" );
}

void setupui::slotAwardClearButtonClicked()
{
    qWarning( "setupui::slotAwardClearButtonClicked(): Not implemented yet" );
}

void setupui::slotAwardRemoveButtonClicked()
{
    qWarning( "setupui::slotAwardRemoveButtonClicked(): Not implemented yet" );
}

void setupui::slotAddDxClusterPushButtonClicked()
{
    qWarning( "setupui::slotAddDxClusterPushButtonClicked(): Not implemented yet" );
}

void setupui::slotClearDxClusterPushButtonClicked()
{
    qWarning( "setupui::slotClearDxClusterPushButtonClicked(): Not implemented yet" );
}

void setupui::slotAwardSelected(QListViewItem*)
{
    qWarning( "setupui::slotAwardSelected(QListViewItem*): Not implemented yet" );
}

void setupui::slotAwardRightButtonFromList(QListViewItem*,const QPoint&)
{
    qWarning( "setupui::slotAwardRightButtonFromList(QListViewItem*,const QPoint&): Not implemented yet" );
}

void setupui::slotAwardDelete(int)
{
    qWarning( "setupui::slotAwardDelete(int): Not implemented yet" );
}

#include "setupui.moc"
