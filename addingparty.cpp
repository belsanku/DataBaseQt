#include "addingparty.h"
#include "ui_addingparty.h"

AddingParty::AddingParty(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddingParty)
{
    ui->setupUi(this);
}

AddingParty::~AddingParty()
{
    delete ui;
}

void AddingParty::on_declineParty_clicked()
{
    this->close();
}

void AddingParty::on_acceptParty_clicked()
{

    partyDB = QSqlDatabase::addDatabase("QSQLITE", "Third");
    partyDB.setDatabaseName("./PartyDatabase.db");

    if (partyDB.open())
    {
        qDebug("open");
    }else
        qDebug("open error");

    partyQuery = new QSqlQuery(partyDB);

    QString partyName = ui->nameLineEdit->text();

    QString partyDate = ui->partyDateEdit->text();
    //qDebug()<<partyDate;

    QString appearenceNeed = ui->apperanceLineEdit->text();

    QString partyPlace = ui->placeLineEdit->text();

    QString partyInfoGroup = ui->infoGroupLineEdit->text();

    partyQuery->prepare("INSERT INTO Мероприятия(Название, Дата_Проведения, Явка, Место, Группы) VALUES (:partyName, :partyDate, :partyAppearence, :partyPlace, :party);");
    partyQuery->bindValue(":partyName", partyName);
    partyQuery->bindValue(":partyDate", partyDate);
    partyQuery->bindValue(":partyAppearence", appearenceNeed);
    partyQuery->bindValue(":partyPlace", partyPlace);
    partyQuery->bindValue(":party", partyInfoGroup);

    if( !partyQuery->exec())
            qDebug() << "Unable to add in table:\n" << partyQuery->lastError();
    partyQuery->clear();

    QString FilePath = "D:/qt/CourseWork/source/Gallery/" + partyName + "/";

    //qDebug()<<FilePath;

    QDir dir;

    if(dir.mkpath(FilePath))
        qDebug("Folder created");
    if(dir.exists(FilePath))
        qDebug("Folder exists");

    this->close();
}
