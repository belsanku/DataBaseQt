#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addingstudent.h"
#include "author.h"
#include "admin.h"
#include "addingparty.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);

    connect(ui->Authors, SIGNAL(triggered()), this, SLOT(on_Authors_clicked()));
    connect(ui->OpenFile, SIGNAL(triggered()), this, SLOT(on_OpenFile_clicked()));
    connect(ui->SaveFile, SIGNAL(triggered()), this, SLOT(on_SaveFile_clicked()));
    connect(ui->NewPassword, SIGNAL(triggered()), this, SLOT(on_NewPassword_clicked()));

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./DataBase.db");

    if (db.open())
    {
        qDebug("open");
    }else
        qDebug("open error");

    query = new QSqlQuery(db);
    query->exec("CREATE TABLE Журнал(ФИО TEXT, Дата_Рождения DATE, Группа TEXT, Фотография TEXT, Почта TEXT, Телефон_Родителей TEXT);");

    if( !query->exec())
            qDebug() << "Unable to create table:\n" << query->lastError();

    query->clear();
    model =  new QSqlTableModel(this, db);
    model->setTable("Журнал");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();

    ui->DBnamespace->setModel(model);

    partyDB = QSqlDatabase::addDatabase("QSQLITE", "Second");
    partyDB.setDatabaseName("./PartyDatabase.db");

    if (partyDB.open())
    {
        qDebug("open");
    }else
        qDebug("open error");

    partyQuery = new QSqlQuery(partyDB);
    partyQuery->exec("CREATE TABLE Мероприятия(Название TEXT, Дата_Проведения DATE, Явка TEXT, Место TEXT, Группы TEXT);");

    if( !partyQuery->exec())
            qDebug() << "Unable to create table:\n" << partyQuery->lastError();

    partyQuery->clear();
    partyModel =  new QSqlTableModel(this, partyDB);
    partyModel->setTable("Мероприятия");
    partyModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    partyModel->select();

    ui->partyTableView->setModel(partyModel);

    updateComboBox();
    update_PartyStudents_combo();

    QString folderName = ui->ChooseParty->currentText();
    //qDebug()<<folderName;
    QString infoImagesPath = "D:/qt/CourseWork/source/Gallery/" + folderName + "/images.txt";
    //qDebug()<<infoImagesPath;

    QFile file(infoImagesPath);


    if(file.open(QIODevice::ReadOnly |QIODevice::Text))
        {
            while(!file.atEnd())
            {
                //читаем строку
                QString str = file.readLine();
                str.remove(str.length()-1, str.length());
                //Делим строку на слова разделенные пробелом
                images.push_back(str);
            }

        }
        else
        {
            qDebug()<< "can't open the file";
        }
    QPixmap pixmap(images[0]);
    ui->PartyImage->setPixmap(pixmap);

    row = 0;

    QString FullName = model->index(row,0).data().toString();
    ui->FullNameLabel->setText(FullName);

    //Сколько лет
    QString AgeLevel = model->index(row,1).data().toString();
    ui->BirthdayTrueLabel->setText(AgeLevel);

    QDate currentDate = QDate::currentDate();
    QDate oldAge = QDate::fromString(AgeLevel, "dd.MM.yyyy");
    qint64 days = oldAge.daysTo(currentDate);
    days/=365;
    QString actualYears = QString::number(days); // actual conversion
    ui->AgeTrueLabel->setText(actualYears);
    //

    QString numberOfGroup = model->index(row,2).data().toString();
    ui->GroupTrueLabel->setText(numberOfGroup);

    QString ImageSrc = model->index(row, 3).data().toString();
    QPixmap tempPix(ImageSrc);
    ui->ImageLabel->setPixmap(tempPix);

    QString ParentsNumber = model->index(row,5).data().toString();
    ui->NumberTrueLabel->setText(ParentsNumber);

    QString Mail = model->index(row,4).data().toString();
    ui->MailTrueLabel->setText(Mail);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateComboBox()
{

    //КОМБО БОКС С МЕРОПРИЯТИЯМИ
    ui->ChooseParty->clear();
    int size = partyModel->rowCount();
    for (int i = 0; i<size; ++i)
    {
        ui->ChooseParty->addItem(partyModel->index(i,0).data().toString());
    }

}


//ВКЛАДКА АВТОР//
void MainWindow::on_Authors_clicked()
{
    Author* auth = new Author();
    auth->show();
}

//СОХРАНИТЬ БД//        ДОРАБОТАТЬ
void MainWindow::on_SaveFile_clicked()
{
    QString SaveDataBase = QFileDialog::getSaveFileName(0, "Сохранить базу данных", QDir::currentPath(), "Файлы баз данных (*.db)");
    QFile SaveDB(SaveDataBase);
    if( SaveDB.open( QIODevice::WriteOnly ) ) {
            QDataStream stream( &SaveDB );
            stream << QObject::trUtf8( "Проверка!" );
            SaveDB.close();
        }

}

//ОТКРЫТЬ БД//
void MainWindow::on_OpenFile_clicked()
{
    QString NewDataBaseName = QFileDialog::getOpenFileName(0, "Открыть базу данных", QDir::currentPath(), "Файлы баз данных (*.db)");
    db.setDatabaseName(NewDataBaseName);

    if (db.open())
    {
        qDebug("open");
    }else
        qDebug("open error");
}

//ИЗМЕНИТЬ ПАРОЛЬ//
void MainWindow::on_NewPassword_clicked()
{
    admin *newAdmin = new admin();
    newAdmin->show();
}

//ДОБАВИТЬ СТУДЕНТА//
void MainWindow::on_addStudent_clicked()
{
    AddingStudent *newStudent = new AddingStudent();
    newStudent->show();

    model =  new QSqlTableModel(this, db);
    model->setTable("Журнал");
    model->select();

    ui->DBnamespace->setModel(model);

    create_Students_folder();
}

//УДАЛИТЬ СТУДЕНТА//
void MainWindow::on_deleteStudent_clicked()
{
    model->removeRow(row);
    model =  new QSqlTableModel(this, db);
    model->setTable("Журнал");
    model->select();

    ui->DBnamespace->setModel(model);

    create_Students_folder();
}

//НАЖАТИЕ НА ТАБЛИЦУ//
void MainWindow::on_DBnamespace_clicked(const QModelIndex &index)
{
    row = index.row();

    QString FullName = model->index(row,0).data().toString();
    ui->FullNameLabel->setText(FullName);

    //Сколько лет
    QString AgeLevel = model->index(row,1).data().toString();
    ui->BirthdayTrueLabel->setText(AgeLevel);

    QDate currentDate = QDate::currentDate();
    QDate oldAge = QDate::fromString(AgeLevel, "dd.MM.yyyy");
    qint64 days = oldAge.daysTo(currentDate);
    days/=365;
    QString actualYears = QString::number(days); // actual conversion
    ui->AgeTrueLabel->setText(actualYears);
    //

    QString numberOfGroup = model->index(row,2).data().toString();
    ui->GroupTrueLabel->setText(numberOfGroup);

    QString ImageSrc = model->index(row, 3).data().toString();
    QPixmap pixmap(ImageSrc);
    ui->ImageLabel->setPixmap(pixmap);

    QString ParentsNumber = model->index(row,5).data().toString();
    ui->NumberTrueLabel->setText(ParentsNumber);

    QString Mail = model->index(row,4).data().toString();
    ui->MailTrueLabel->setText(Mail);

}

void MainWindow::on_editButton_clicked()
{

}

void MainWindow::create_Students_folder()
{
    QString folderPath = "D:/qt/CourseWork/source/Gallery/AllStudentsModelInfo/";
    QDir dir;
    if(dir.mkpath(folderPath))
           qDebug("folder created");
       if(dir.exists(folderPath))
           qDebug("folder exists");

       folderPath+="studentsInfo.txt";
    QFile studentInfoFile(folderPath);

    QTextStream ts(&studentInfoFile);

    if (studentInfoFile.open(QIODevice::WriteOnly))
    {
        for (int i = 0; i<model->rowCount(); ++i){
        QString FullName = model->index(i,0).data().toString() + " " + model->index(i,2).data().toString() + "\n";
        ts<<FullName;
        }
        studentInfoFile.close();
    }
}


void MainWindow::update_PartyStudents_combo()
{
    ui->addingPartyStudents->clear();
    int n = model->rowCount();
    ui->addingPartyStudents->addItem("");
    for (int i = 0; i<n; ++i)
    {
        ui->addingPartyStudents->addItem(model->index(i,0).data().toString() + " " + model->index(i,2).data().toString());
    }
}

//ОБНОВИТЬ ТАБЛИЦУ//
void MainWindow::on_UpdateButton_clicked()
{
    model =  new QSqlTableModel(this, db);
    model->setTable("Журнал");
    model->select();

    ui->DBnamespace->setModel(model);

    create_Students_folder();

    //КОМБО БОКС СО СТУДЕНТАМИ
    update_PartyStudents_combo();
}

void MainWindow::on_sortDBby_clicked()
{
    int index = ui->selectSortBy->currentIndex();
    if (index == 0)
    {
        model->sort(2, Qt::AscendingOrder);
    }else if (index == 1)
    {
        model->sort(0, Qt::AscendingOrder);
    }
}

void MainWindow::on_resetButton_clicked()
{
    ui->selectSortBy->setCurrentIndex(0);
    model->setTable("Журнал");
    model->select();
    ui->DBnamespace->setModel(model);
}

void MainWindow::on_partyButton_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

//НАЖАТИЕ НА ЭЛЕМЕНТ ТАБЛИЦЫ//
void MainWindow::on_partyTableView_clicked(const QModelIndex &index)
{
    partyRow = index.row();
    ui->ChooseParty->setCurrentIndex(partyRow);

    QString PartyName = partyModel->index(partyRow,0).data().toString();
    ui->SetPartyName->setText(PartyName);

    QString PartyDate = partyModel->index(partyRow,1).data().toString();
    ui->SetPartyDate->setText(PartyDate);

    QString PartyAppearence = partyModel->index(partyRow,2).data().toString();
    ui->SetPartyAppearence->setText(PartyAppearence);

    QString PartyPlace = partyModel->index(partyRow,3).data().toString();
    ui->SetPartyPlace->setText(PartyPlace);

    QString PartyGroup = partyModel->index(partyRow,4).data().toString();
    ui->SetPartyGroup->setText(PartyGroup);

    ui->listWidget->clear();
    ui->listWidget->addItem(PartyName + ":");

    QString infoImagesPath = "D:/qt/CourseWork/source/Gallery/" + PartyName + "/partyStudents.txt";

    QFile file(infoImagesPath);
    QList<QString> texts;
    QListWidgetItem *item = new QListWidgetItem();
    if(file.open(QIODevice::ReadOnly |QIODevice::Text))
        {
            while(!file.atEnd())
            {
                //читаем строку
                QString str = file.readLine();
                str.remove(str.length()-1, str.length());
                //Делим строку на слова разделенные пробелом
                texts.push_back(str);
            }

        }
        else
        {
            qDebug()<< "can't open the file";
        }

    for (int i = 1; i<texts.size(); ++i)
    {
        ui->listWidget->addItem(texts[i]);
    }
    //updateComboBox();
}

void MainWindow::on_addingPartyStudents_currentIndexChanged(int index)
{
    int current = partyRow;
    QString AshesOne = ui->addingPartyStudents->currentText();
    qDebug()<<AshesOne;
    //ui->addingPartyStudents->removeItem(index);
    qDebug()<<index;
    ui->listWidget->addItem(AshesOne);
}

//ФУНКЦИЯ УДАЛЕНИЯ ПАПКИ//
int removeFolder(QDir & dir)
{
  int res = 0;
  //Получаем список каталогов
  QStringList lstDirs = dir.entryList(QDir::Dirs |
                  QDir::AllDirs |
                  QDir::NoDotAndDotDot);
  //Получаем список файлов
  QStringList lstFiles = dir.entryList(QDir::Files);

  //Удаляем файлы
  foreach (QString entry, lstFiles)
  {
   QString entryAbsPath = dir.absolutePath() + "/" + entry;
   QFile::setPermissions(entryAbsPath, QFile::ReadOwner | QFile::WriteOwner);
   QFile::remove(entryAbsPath);
  }

  //Для папок делаем рекурсивный вызов
  foreach (QString entry, lstDirs)
  {
   QString entryAbsPath = dir.absolutePath() + "/" + entry;
   QDir dr(entryAbsPath);
   removeFolder(dr);
  }

  //Удаляем обрабатываемую папку
  if (!QDir().rmdir(dir.absolutePath()))
  {
    res = 1;
  }
  return res;
}

//УДАЛИТЬ МЕРОПРИЯТИЕ ИЗ ТАБЛИЦЫ//
void MainWindow::on_deletePartyButton_clicked()
{
    QString PartyName = partyModel->index(partyRow,0).data().toString();
    QDir dir;

    dir = "D:/qt/CourseWork/source/Gallery/" + PartyName + "/";
    removeFolder(dir);

    partyModel->removeRow(partyRow);

    partyModel =  new QSqlTableModel(this, partyDB);
    partyModel->setTable("Мероприятия");
    partyModel->select();
    partyModel->sort(1, Qt::DescendingOrder);
    ui->partyTableView->setModel(partyModel);
}


//ДОБАВИТЬ МЕРОПРИЯТИЕ В ТАБЛИЦУ//
void MainWindow::on_addPartyButton_clicked()
{
    AddingParty *newParty = new AddingParty;
    newParty->show();
}


//ОБНОВИТЬ ТАБЛИЦУ МЕРОПРИЯТИЙ//
void MainWindow::on_updatePartyButton_clicked()
{
    partyModel =  new QSqlTableModel(this, partyDB);
    partyModel->setTable("Мероприятия");
    partyModel->select();
    partyModel->sort(1, Qt::DescendingOrder);
    ui->partyTableView->setModel(partyModel);

    updateComboBox();
}


//НОВОЕ ФОТО В МЕРОПРИЯТИЯХ//
void MainWindow::on_AddPhoto_clicked()
{

    QString folderName = ui->ChooseParty->currentText();
    QString infoImagesPath = "D:/qt/CourseWork/source/Gallery/" + folderName + "/images.txt";

    QFile file(infoImagesPath);

    QString PathToImage = QFileDialog::getOpenFileName(0, "Открыть изображение", QDir::currentPath(), "Файлы изображений (*.png *.jpg *.bmp)");

    if (file.open(QIODevice::Append)) {
        QTextStream out(&file);
        out<<PathToImage;
        out<<"\n";
    }

    file.close();
}


//ВЫБРАТЬ МЕРОПРИЯТИЕ В COMBO BOX//
void MainWindow::on_ChooseParty_currentIndexChanged(int index)
{
    partyRow = index;

    QString PartyName = partyModel->index(partyRow,0).data().toString();
    ui->SetPartyName->setText(PartyName);

    QString PartyDate = partyModel->index(partyRow,1).data().toString();
    ui->SetPartyDate->setText(PartyDate);

    QString PartyAppearence = partyModel->index(partyRow,2).data().toString();
    ui->SetPartyAppearence->setText(PartyAppearence);

    QString PartyPlace = partyModel->index(partyRow,3).data().toString();
    ui->SetPartyPlace->setText(PartyPlace);

    QString PartyGroup = partyModel->index(partyRow,4).data().toString();
    ui->SetPartyGroup->setText(PartyGroup);

    images.clear();
    ui->PartyImage->clear();
    QString folderName = ui->ChooseParty->currentText();
    QString infoImagesPath = "D:/qt/CourseWork/source/Gallery/" + folderName + "/images.txt";

    QFile file(infoImagesPath);


    if(file.open(QIODevice::ReadOnly |QIODevice::Text))
        {
            while(!file.atEnd())
            {
                //читаем строку
                QString str = file.readLine();
                str.remove(str.length()-1, str.length());
                //Делим строку на слова разделенные пробелом
                images.push_back(str);
            }

        }
        else
        {
            qDebug()<< "can't open the file";
        }
    if (images.isEmpty())
    {

        ui->PartyImage->clear();
        ui->PartyImage->setText("Картинка отсутствует.");
        ui->BeforeImage->hide();
        ui->NextImage->hide();
        qDebug()<<"Empty";

    }else{

    QPixmap pixmap(images[0]);
    ui->PartyImage->setPixmap(pixmap);
    ui->BeforeImage->show();
    ui->NextImage->show();

    }
}


//ПРЕДЫДУЩАЯ КАРТИНКА В МЕРОПРИЯТИЯХ//
void MainWindow::on_BeforeImage_clicked()
{   if (images.isEmpty()){
        qDebug()<<"Empty";
    }else{
    int maxSize = images.size();

    if (cnt == 0)
    {
        cnt = maxSize-1;
        QPixmap currentImage(images[cnt]);
        ui->PartyImage->clear();
        ui->PartyImage->setPixmap(currentImage);
    }else{
        cnt--;
        QPixmap currentImage(images[cnt]);
        ui->PartyImage->clear();
        ui->PartyImage->setPixmap(currentImage);
    }
    }
}

//СЛЕДУЮЩАЯ КАРТИКА В МЕРОПРИЯТИЯХ//
void MainWindow::on_NextImage_clicked()
{   if (images.isEmpty()){
        qDebug()<<"Empty";
    }else{
    int maxSize = images.size();

    if (cnt<maxSize-1)
    {
        cnt++;
        QPixmap currentImage(images[cnt]);
        ui->PartyImage->clear();
        ui->PartyImage->setPixmap(currentImage);
    }else{
        cnt = 0;
        QPixmap currentImage(images[cnt]);;
        ui->PartyImage->clear();
        ui->PartyImage->setPixmap(currentImage);
    }
    }
}


/*void MainWindow::on_addingPartyStudents_highlighted(int index)
{

}*/



void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    foreach(item, items)
    {
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
    }
}

void MainWindow::on_saveInfo_clicked()
{


    QList<QString> texts;

    QList<QListWidgetItem *> items =
          ui->listWidget->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);

    foreach(QListWidgetItem *item, items)
      texts.append(item->text());

    QString folderName = texts[0];
    folderName.chop(1);
    QString filePath = "D:/qt/CourseWork/source/Gallery/" + folderName + "/";
    filePath += "partyStudents.txt";
    QFile file(filePath);

    QTextStream ts(&file);
    ts.setCodec("UTF-8");

    if (file.open(QIODevice::WriteOnly))
    {
        for (int i = 0; i<texts.size(); ++i){
        ts<<texts[i]+"\n";
        }
        file.close();
    }


    qDebug()<<texts;

}
















