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

    //Инициализация триггеров
    connect(ui->Authors, SIGNAL(triggered()), this, SLOT(on_Authors_clicked()));
    connect(ui->OpenFile, SIGNAL(triggered()), this, SLOT(on_OpenFile_clicked()));
    connect(ui->SaveFile, SIGNAL(triggered()), this, SLOT(on_SaveFile_clicked()));
    connect(ui->NewPassword, SIGNAL(triggered()), this, SLOT(on_NewPassword_clicked()));


    //Создание базы данных студентов
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


    //Создание базы данных мероприятий
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

    QString BigCatPath = ":/img/source/goodMorning.png";
    QPixmap BigCat(BigCatPath);
    ui->BigCatLabel->setPixmap(BigCat);

    updateComboBox();
    update_PartyStudents_combo();
    update_successComboBox();
    update_akatsukiComboBox();
    update_leaderOfAkatsukiComboBox();

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


    checkSuccessFile();
    checkAkatsukiSuccessFile();

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSuccessTableView()
{

    QFile file("D:/qt/CourseWork/source/SuccessTable/successTable.csv");
       if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
           qDebug() << "File not exists";
       } else {

           QTextStream in(&file);
           successModel->clear();
           for (int i =0; i<7; ++i){
               QStandardItem* newColumn = new QStandardItem(QString(ColumnNames[i]));
               successModel->setHorizontalHeaderItem(i, newColumn);

           }
           while (!in.atEnd())
           {
               QString line = in.readLine();

               QList<QStandardItem *> standardItemsList;

               for (QString item : line.split(";")) {
                   standardItemsList.append(new QStandardItem(item));
               }
               successModel->insertRow(successModel->rowCount(), standardItemsList);
           }
           file.close();
       }

       ui->successTableView->setColumnWidth(0,490);
       ui->successTableView->setColumnWidth(1, 100);
       for (int i =2; i<7; ++i)
       {
           ui->successTableView->setColumnWidth(i, 60);
       }

       ui->successTableView->setColumnHidden(7,true);
}

//СОХРАНЕНИЕ ТАБЛИЦЫ УСПЕВАЕМОСТИ//
void MainWindow::saveSuccessTableView()
{
    QString textData;
    int rows = successModel->rowCount();
    int columns = successModel->columnCount();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {

                textData += successModel->data(successModel->index(i,j)).toString();
                textData += "; " ;
        }
        textData += "\n";
    }

    QString folderPath = "D:/qt/CourseWork/source/SuccessTable/";
    QDir dir;
    if(dir.mkpath(folderPath))
           qDebug("folder created");
       if(dir.exists(folderPath))
           qDebug("folder exists");

       folderPath+="successTable.csv";

    QFile csvFile(folderPath);
    if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {

        QTextStream out(&csvFile);
        out << textData;

        csvFile.close();
    }
}

void MainWindow::createSuccessTableView()
{

    int size = model->rowCount();
    for (int i = 0; i<size; ++i)
    {
        StudentName.push_back(model->index(i,0).data().toString());
        StudentGroup.push_back(model->index(i,2).data().toString());
    }

    ColumnNames<<"ФИО"<< "Группа"<< "ВПиЧМВ"<< "ОС"<< "МатЛог"<< "ТМО"<< "Сессия";

    successModel = new QStandardItemModel(size,7);
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < 7; ++column) {
            QStandardItem *item = new QStandardItem(QString("").arg(row).arg(column));
            successModel->setItem(row, column, item);
        }
    }

    ui->successTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->successTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    for (int i =0; i<7; ++i){
        QStandardItem* newColumn = new QStandardItem(QString(ColumnNames[i]));
        successModel->setHorizontalHeaderItem(i, newColumn);

    }
    ui->successTableView->setModel(successModel);
}

void MainWindow::fillTableView()
{
    int size = model->rowCount();
    ui->successTableView->setColumnWidth(0,490);
    ui->successTableView->setColumnWidth(1, 100);
    for (int i =2; i<7; ++i)
    {
        ui->successTableView->setColumnWidth(i, 60);
    }

    for (int i = 0; i<size; ++i){
        QStandardItem* newElement = new QStandardItem(QString(StudentName[i]));
        QStandardItem* newTempElement = new QStandardItem(QString(StudentGroup[i]));
        successModel->setItem(i,0,newElement);
        successModel->setItem(i,1,newTempElement);
    }
    successModel->sort(1, Qt::AscendingOrder);
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
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Information, "Удаление", "Вы уверены, что хотите удалить студента?", QMessageBox::Ok| QMessageBox::Cancel, this);

    if (msgBox->exec() == QMessageBox::Ok){
    model->removeRow(row);
    model =  new QSqlTableModel(this, db);
    model->setTable("Журнал");
    model->select();

    ui->DBnamespace->setModel(model);

    create_Students_folder();
    update_successComboBox();
    delete msgBox;
    }else delete msgBox;
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
    update_successComboBox();
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
    //int current = partyRow;
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
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Information, "Удаление", "Вы уверены, что хотите удалить мероприятие?", QMessageBox::Ok| QMessageBox::Cancel, this);
    if (msgBox->exec() == QMessageBox::Ok){
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

    delete msgBox;
    }else delete msgBox;
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


//УДАЛИТЬ СТУДЕНТА ИЗ СПИСКА ПОСЕТИВШИХ МЕРОПРИЯТИЕ//
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Information, "Удаление", "Вы уверены, что хотите удалить студента?", QMessageBox::Ok| QMessageBox::Cancel, this);
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    if (msgBox->exec() == QMessageBox::Ok){
        foreach(item, items)
        {
            delete ui->listWidget->takeItem(ui->listWidget->row(item));
        }
        delete msgBox;
    }else delete msgBox;
}

//СОХРАНИТЬ СПИСОК СТУДЕНТОВ, ПОСЕТИВШИХ МЕРОПРИЯТИЕ//
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

//КНОПКА ПЕРЕХОДА НА УСПЕВАЕМОСТЬ//
void MainWindow::on_succButton_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
    update_successComboBox();
}

//КНОПКА ПЕРЕХОДА НА СТАРОСТ//
void MainWindow::on_masterButton_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

//КНОПКА СОХРАНЕНИЯ ТАБЛИЦЫ УСПЕВАЕМОСТИ//
void MainWindow::on_saveSuccessTableViewButton_clicked()
{
    saveSuccessTableView();
}

//КНОПКА ПРОСТАВЛЕНИЯ РЕЗУЛЬТАТОВ СЕССИИ//
void MainWindow::on_setResultSession_clicked()
{

    int size = successModel->rowCount();

    for (int i = 0; i<size; ++i)
    {
        double result = 0;
        int twoCnt = 0;

        for (int j = 2; j<6; ++j)
        {
            result+=successModel->index(i,j).data().toInt();
            if (successModel->index(i,j).data().toInt() == 2)
            {
                twoCnt++;
            }
        }
        QString temp = "";
        if (twoCnt>0)
        {
            temp = "Плох.";

        }else{

        if (result/4 > 4.5) temp = "Отл.";
        else if (result/4 > 3.5 && result/4 <= 4.5) temp = "Хор.";
        else if (result/4 > 2.99 && result/4 <= 3.5) temp = "Удовл.";

        }

        QStandardItem* mark = new QStandardItem(temp);
        successModel->setItem(i,6,mark);
        ui->successTableView->setColumnHidden(7, true);
    }
}

//КНОПКА ПОКАЗА НЕУСПЕВАЮЩИХ СТУДЕНТОВ//
void MainWindow::on_unsuccessfulStudentsTableViewButton_clicked()
{
    for (int i =0; i<successModel->rowCount(); ++i)
    {
        QString currentMark = successModel->index(i,6).data().toString();
        if (currentMark != " Плох.")
        {
            ui->successTableView->setRowHidden(i, true);
        }
    }
}

//СБРОСИТЬ ТАБЛИЦУ УСПЕВАЕМОСТИ//
void MainWindow::on_resetSuccessTableView_clicked()
{
    for (int i =0; i<successModel->rowCount(); ++i)
    {
            ui->successTableView->setRowHidden(i, false);
    }
}

//ОБНОВИТЬ ТАБЛИЦУ УСПЕВАЕМОСТИ//
void MainWindow::update_successComboBox()
{

    QVector<QString>elements;

    for (int i = 0; i<model->rowCount(); ++i)
    {
        uniqueGroup.insert(model->index(i,2).data().toString());
    }

    foreach (const QString &value, uniqueGroup)
        elements.push_back(value);

    for (int i = 0; i<elements.size(); ++i)
    {
        ui->successComboBox->addItem(elements[i]);
    }
}

//ПОКАЗАТЬ ОПРЕДЕЛЕННУЮ ГРУППУ В УСПЕВАЕМОСТИ//
void MainWindow::on_showGroupButton_clicked()
{
   on_resetSuccessTableView_clicked();
   QString currentGroup = ui->successComboBox->currentText();

   for (int i =0; i<successModel->rowCount(); ++i)
   {
       QString Group = successModel->index(i,1).data().toString();

       Group = Group.trimmed();

       if (Group != currentGroup)
       {
           ui->successTableView->setRowHidden(i, true);
       }
   }

}

//КНОПКА ПОИСКА СТУДЕНТА ПО ФИО//
void MainWindow::on_searchButton_clicked()
{
   on_resetSuccessTableView_clicked();
   if (ui->searchByFIO->text() != "")
   {
       QString FIO = ui->searchByFIO->text();
       for (int i =0; i<successModel->rowCount(); ++i)
       {
           QString currentFIO = successModel->index(i,0).data().toString();

           if (FIO != currentFIO)
           {
               ui->successTableView->setRowHidden(i, true);
           }
       }
   }
}

//СОЗДАНИЕ ПДФ ФАЙЛА С НЕУСПЕВАЮЩИМИ//
void MainWindow::on_createPdfSuccessTableViewButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить отчёт", QDir::currentPath(), "*.pdf");

        if (filename != "") {
            QPdfWriter pdf(filename);
            QPainter painter(&pdf);
            int sign = 0;
            painter.drawText(20,50,QString("Список неуспевщих студентов по итогам сессии:"));

            for (int i =0, x = 20, y = 500; i<successModel->rowCount(); ++i)
            {
                QString currentMark = successModel->index(i,6).data().toString();
                if (currentMark == " Плох.")
                {
                    QString currentName = successModel->index(i,0).data().toString();

                    if (currentName.length()>=30) currentName+="\t";
                    else currentName+="\t\t";

                    QString currentGroup = successModel->index(i,1).data().toString();
                    currentGroup = "Группа:"+currentGroup;
                    painter.drawText(x,y, currentName + currentGroup);
                    y+=200; sign = y;
                }
            }
            painter.drawText(20, sign+300, "ПОДПИСЬ: ");
        }
}


//ЗАПОЛНИТЬ ТАБЛИЦУ СТАРОСТ//
void MainWindow::createAkatsukiTableView()
{
    AkatsukiColumnNames<<"ФИО"<<"Группа"<<"Роль";

    leaderModel = new QStandardItemModel;
    for (int i =0; i<3; ++i){
        QStandardItem* newColumn = new QStandardItem(QString(AkatsukiColumnNames[i]));
        leaderModel->setHorizontalHeaderItem(i, newColumn);

    }

    ui->akatsukiTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->akatsukiTableView->setSelectionMode(QAbstractItemView::SingleSelection);


    ui->akatsukiTableView->setModel(leaderModel);

    ui->akatsukiTableView->setColumnWidth(0,335);
    ui->akatsukiTableView->setColumnWidth(1,85);
    ui->akatsukiTableView->setColumnWidth(2,85);
}

//КОМБО БОКС С ГРУППАМИ В СТАРОСТАХ//
void MainWindow::update_akatsukiComboBox()
{
    ui->chooseLeaderOfAkatsuki->clear();
    QVector<QString>elements;

    for (int i = 0; i<model->rowCount(); ++i)
    {
        uniqueGroup.insert(model->index(i,2).data().toString());
    }

    foreach (const QString &value, uniqueGroup)
        elements.push_back(value);

    for (int i = 0; i<elements.size(); ++i)
    {
        ui->chooseGroupOfAkatsuki->addItem(elements[i]);
    }
    update_leaderOfAkatsukiComboBox();
}

//КОМБО БОКС СО СТУДЕНТАМИ В СТАРОСТАХ//
void MainWindow::update_leaderOfAkatsukiComboBox()
{
    ui->chooseLeaderOfAkatsuki->clear();
    QVector<QString>students;

    QString currentGroup = ui->chooseGroupOfAkatsuki->currentText();

    for (int i = 0; i<model->rowCount(); ++i)
    {
        if (model->index(i,2).data().toString() == currentGroup)
        {
            students.push_back(model->index(i,0).data().toString());
        }
    }

    for (int i =0; i<students.size(); ++i)
    {
        ui->chooseLeaderOfAkatsuki->addItem(students[i]);
    }
}

//ПРОВЕРКА СУЩЕСТВОВАНИЯ ФАЙЛА ТАБЛИЦЫ С УСПЕВАЕМОСТЬЮ//
void MainWindow::checkSuccessFile()
{
    QFile successFile("D:/qt/CourseWork/source/SuccessTable/successTable.csv");
       if ( !successFile.open(QFile::ReadOnly | QFile::Text) ) {
           qDebug() << "File not exists";
           createSuccessTableView();
           fillTableView();
       } else {
           qDebug()<<"Trying to read...";
           createSuccessTableView();
           readSuccessTableView();
       }
}

//ПРИ ИЗМЕНЕНИИ КОМБО БОКСА С ГРУППАМИ - МЕНЯЕТСЯ КОМБО БОКС СО СТУДЕНТАМИ//
void MainWindow::on_chooseGroupOfAkatsuki_currentTextChanged(const QString &arg1)
{
    ui->chooseLeaderOfAkatsuki->clear();
    QVector<QString>students;

    QString currentGroup = arg1;

    for (int i = 0; i<model->rowCount(); ++i)
    {
        if (model->index(i,2).data().toString() == currentGroup)
        {
            students.push_back(model->index(i,0).data().toString());
        }
    }

    for (int i =0; i<students.size(); ++i)
    {
        ui->chooseLeaderOfAkatsuki->addItem(students[i]);
    }
}

void MainWindow::on_addAkatsukiLeader_clicked()
{
    QList<QStandardItem *> standardItemsList;

    QStandardItem* NameOfStudent = new QStandardItem(ui->chooseLeaderOfAkatsuki->currentText());
    standardItemsList.push_back(NameOfStudent);

    QStandardItem* NameOfGroup = new QStandardItem(ui->chooseGroupOfAkatsuki->currentText());
    standardItemsList.push_back(NameOfGroup);

    QStandardItem* Role = new QStandardItem(QString("Староста"));
    standardItemsList.push_back(Role);

    leaderModel->insertRow(leaderModel->rowCount(), standardItemsList);

    ui->akatsukiTableView->setColumnWidth(0,335);
    ui->akatsukiTableView->setColumnWidth(1,85);
    ui->akatsukiTableView->setColumnWidth(2,85);
}


void MainWindow::on_saveAkatsukiTableButton_clicked()
{
    QString textData;
    int rows = leaderModel->rowCount();
    int columns = leaderModel->columnCount();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {

                textData += leaderModel->data(leaderModel->index(i,j)).toString();
                textData += "; " ;
        }
        textData += "\n";
    }

    QString folderPath = "D:/qt/CourseWork/source/LeaderTable/";
    QDir dir;
    if(dir.mkpath(folderPath))
           qDebug("folder created");
       if(dir.exists(folderPath))
           qDebug("folder exists");

       folderPath+="leaderTable.csv";

    QFile csvFile(folderPath);
    if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {

        QTextStream out(&csvFile);
        out << textData;

        csvFile.close();
    }
}

void MainWindow::readAkatsukiTable()
{
    QFile file("D:/qt/CourseWork/source/LeaderTable/leaderTable.csv");
       if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
           qDebug() << "File not exists";
       } else {

           QTextStream in(&file);
           leaderModel->clear();
           for (int i =0; i<3; ++i){
               QStandardItem* newColumn = new QStandardItem(QString(AkatsukiColumnNames[i]));
               leaderModel->setHorizontalHeaderItem(i, newColumn);

           }
           while (!in.atEnd())
           {
               QString line = in.readLine();

               QList<QStandardItem *> standardItemsList;

               for (QString item : line.split(";")) {
                   standardItemsList.append(new QStandardItem(item));
               }
               leaderModel->insertRow(leaderModel->rowCount(), standardItemsList);
           }
           file.close();
       }

       ui->akatsukiTableView->setColumnWidth(0,335);
       ui->akatsukiTableView->setColumnWidth(1,85);
       ui->akatsukiTableView->setColumnWidth(2,85);
       ui->akatsukiTableView->setColumnHidden(3,true);
}

void MainWindow::checkAkatsukiSuccessFile()
{
    QFile successFile("D:/qt/CourseWork/source/LeaderTable/leaderTable.csv");
       if ( !successFile.open(QFile::ReadOnly | QFile::Text) ) {
           qDebug() << "File not exists";
           createAkatsukiTableView();
       } else {
           qDebug()<<"Trying to read...";
           createAkatsukiTableView();
           readAkatsukiTable();
       }
}

void MainWindow::on_akatsukiTableView_doubleClicked(const QModelIndex &index)
{
    leaderRow = index.row();
}

void MainWindow::on_akatsukiTableView_clicked(const QModelIndex &index)
{
    leaderRow = index.row();
}

void MainWindow::on_akatsukiTableView_activated(const QModelIndex &index)
{
    leaderRow = index.row();
}

void MainWindow::on_deleteLeaderButton_clicked()
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Information, "Удаление", "Вы уверены, что хотите удалить cтаросту?", QMessageBox::Ok| QMessageBox::Cancel, this);
    if (msgBox->exec() == QMessageBox::Ok){
    leaderModel->removeRow(leaderRow);
    ui->akatsukiTableView->setModel(leaderModel);
    delete msgBox;
    }else delete msgBox;
}

void MainWindow::on_actionInfo_triggered()
{
    QProcess *process = new QProcess;
     QStringList args;
     args << QLatin1String("-collectionFile")
         << QLatin1String("helpCollection.qhc")
         << QLatin1String("-enableRemoteControl");
     process->start(QLatin1String("assistant"), args);
     if (!process->waitForStarted())
         return;
}
