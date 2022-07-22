/* Copyright (c) 2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "xprocessmoduleswidget.h"
#include "ui_xprocessmoduleswidget.h"

XProcessModulesWidget::XProcessModulesWidget(QWidget *pParent) :
    XShortcutsWidget(pParent),
    ui(new Ui::XProcessModulesWidget)
{
    ui->setupUi(this);

    // mb TODO autorefresh

    g_nProcessId=0;
    g_pXInfoDB=nullptr;
    g_pModel=nullptr;
    g_pOldModel=nullptr;

    memset(shortCuts,0,sizeof shortCuts);
}

XProcessModulesWidget::~XProcessModulesWidget()
{
    delete ui;
}

void XProcessModulesWidget::setData(qint64 nProcessId,bool bReload)
{
    g_nProcessId=nProcessId;

#ifdef QT_DEBUG
    qDebug("XProcessModulesWidget::setData %lld",nProcessId);
#endif

    if(bReload)
    {
        reload();
    }
}

void XProcessModulesWidget::setXInfoDB(XInfoDB *pXInfoDB,bool bReload)
{
    g_pXInfoDB=pXInfoDB;

    if(bReload)
    {
        reload();
    }
}

void XProcessModulesWidget::reload()
{
#ifdef QT_DEBUG
    qDebug("void XProcessModulesWidget::reload()");
#endif

    QList<XProcess::MODULE> listModules;
    QList<XProcess::MODULE> *pListModules=nullptr;

    if(g_nProcessId)
    {
        listModules=XProcess::getModulesList(g_nProcessId);
        pListModules=&listModules;
    }
    else if(g_pXInfoDB)
    {
        pListModules=g_pXInfoDB->getCurrentModulesList();
    }

    if(g_nProcessId||g_pXInfoDB)
    {
        g_pOldModel=g_pModel;

        XBinary::MODE modeAddress=XBinary::MODE_32;

        if(sizeof(void *)==8)
        {
            modeAddress=XBinary::MODE_64;
        }
        else
        {
            modeAddress=XBinary::MODE_32;
        }

        qint32 nNumberOfRecords=pListModules->count();

        g_pModel=new QStandardItemModel(nNumberOfRecords,__HEADER_COLUMN_size);

        g_pModel->setHeaderData(HEADER_COLUMN_ADDRESS,Qt::Horizontal,tr("Address"));
        g_pModel->setHeaderData(HEADER_COLUMN_SIZE,Qt::Horizontal,tr("Size"));
        g_pModel->setHeaderData(HEADER_COLUMN_NAME,Qt::Horizontal,tr("Name"));
        g_pModel->setHeaderData(HEADER_COLUMN_FILENAME,Qt::Horizontal,tr("File name"));

        for(qint32 i=0;i<nNumberOfRecords;i++)
        {
            QStandardItem *pItemAddress=new QStandardItem;
            pItemAddress->setText(XBinary::valueToHex(modeAddress,pListModules->at(i).nAddress));
            pItemAddress->setData(pListModules->at(i).nAddress,Qt::UserRole+USERROLE_ADDRESS);
            pItemAddress->setData(pListModules->at(i).nSize,Qt::UserRole+USERROLE_SIZE);
            pItemAddress->setData(pListModules->at(i).sFileName,Qt::UserRole+USERROLE_FILENAME);
            pItemAddress->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_ADDRESS,pItemAddress);

            QStandardItem *pItemSize=new QStandardItem;
            pItemSize->setText(XBinary::valueToHex(XBinary::MODE_32,pListModules->at(i).nSize));
            pItemSize->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_SIZE,pItemSize);

            QStandardItem *pItemName=new QStandardItem;
            pItemName->setText(pListModules->at(i).sName);
            pItemName->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_NAME,pItemName);

            QStandardItem *pItemFileName=new QStandardItem;
            pItemFileName->setText(pListModules->at(i).sFileName);
            pItemFileName->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_FILENAME,pItemFileName);
        }

        ui->tableViewModules->setModel(g_pModel);

        deleteOldStandardModel(&g_pOldModel);
    }
}

void XProcessModulesWidget::registerShortcuts(bool bState)
{
    if(bState)
    {
        if(!shortCuts[SC_DUMPTOFILE])               shortCuts[SC_DUMPTOFILE]                =new QShortcut(getShortcuts()->getShortcut(X_ID_MODULES_DUMPTOFILE),            this,SLOT(_dumpToFileSlot()));
        if(!shortCuts[SC_SHOWIN_FOLDER])            shortCuts[SC_SHOWIN_FOLDER]             =new QShortcut(getShortcuts()->getShortcut(X_ID_MODULES_SHOWIN_FOLDER),         this,SLOT(_showInFolderSlot()));
    }
    else
    {
        for(qint32 i=0;i<__SC_SIZE;i++)
        {
            if(shortCuts[i])
            {
                delete shortCuts[i];
                shortCuts[i]=nullptr;
            }
        }
    }
}

void XProcessModulesWidget::on_pushButtonSave_clicked()
{
    if(g_pModel)
    {
        XShortcutsWidget::saveModel(g_pModel,QString("%1.txt").arg(tr("Modules")));
    }
}

void XProcessModulesWidget::on_pushButtonReload_clicked()
{
    reload();
}

void XProcessModulesWidget::on_tableViewModules_customContextMenuRequested(const QPoint &pos)
{
    QMenu menuContext(this);

    QMenu menuShowIn(tr("Show in"),this);

    QAction actionDumpToFile(tr("Dump to file"),this);
    actionDumpToFile.setShortcut(getShortcuts()->getShortcut(X_ID_MODULES_DUMPTOFILE));
    connect(&actionDumpToFile,SIGNAL(triggered()),this,SLOT(_dumpToFileSlot()));

    QAction actionShowInFolder(tr("Folder"),this);
    actionShowInFolder.setShortcut(getShortcuts()->getShortcut(X_ID_MODULES_SHOWIN_FOLDER));
    connect(&actionShowInFolder,SIGNAL(triggered()),this,SLOT(_showInFolderSlot()));

    menuContext.addAction(&actionDumpToFile);

    menuShowIn.addAction(&actionShowInFolder);

    menuContext.addMenu(&menuShowIn);

    menuContext.exec(ui->tableViewModules->viewport()->mapToGlobal(pos));
}

void XProcessModulesWidget::_dumpToFileSlot()
{
    QString sSaveFileName=QString("%1.bin").arg(tr("Dump")); // TODO rename
    QString sFileName=QFileDialog::getSaveFileName(this,tr("Save dump"),sSaveFileName,QString("%1 (*.bin)").arg(tr("Raw data")));

    if(!sFileName.isEmpty())
    {
        qint32 nRow=ui->tableViewModules->currentIndex().row();

        if((nRow!=-1)&&(g_pModel))
        {
            QModelIndex index=ui->tableViewModules->selectionModel()->selectedIndexes().at(0);

            quint64 nAddress=ui->tableViewModules->model()->data(index,Qt::UserRole+USERROLE_ADDRESS).toString().toULongLong(0,16);
            quint64 nSize=ui->tableViewModules->model()->data(index,Qt::UserRole+USERROLE_SIZE).toString().toULongLong(0,16);

            XProcess pd(g_nProcessId,nAddress,nSize);

            if(pd.open(QIODevice::ReadOnly))
            {
                DialogDumpProcess dd(this,&pd,0,nSize,sFileName,DumpProcess::DT_OFFSET);

                dd.exec();

                pd.close();
            }
        }
    }
}

void XProcessModulesWidget::_showInFolderSlot()
{
    qint32 nRow=ui->tableViewModules->currentIndex().row();

    if((nRow!=-1)&&(g_pModel))
    {
        QModelIndex index=ui->tableViewModules->selectionModel()->selectedIndexes().at(0);

        QString sFilePath=ui->tableViewModules->model()->data(index,Qt::UserRole+USERROLE_FILENAME).toString();

        XOptions::showInFolder(sFilePath);
    }
}
