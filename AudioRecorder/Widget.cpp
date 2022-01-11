#include <QAudioProbe>
#include <QAudioRecorder>
#include "Widget.h"
#include "ui_Widget.h"

static QVariant boxValue(const QComboBox *box)
{
    int idx = box->currentIndex();
    if (idx == -1)
        return QVariant();

    return box->itemData(idx);
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_audioRecorder = new QAudioRecorder( this );
    m_probe = new QAudioProbe( this );

    // 录音设备
    ui->device->addItem( "Default", QVariant() );
    for ( auto &device: m_audioRecorder->audioInputs() ) {
        ui->device->addItem( device, QVariant( device ) );
    }

    // 编码
    ui->codec->addItem( "Default", QVariant() );
    for ( auto &codec: m_audioRecorder->supportedAudioCodecs() ) {
        ui->codec->addItem( codec, QVariant( codec ) );
    }

    // 文件容器格式
    ui->file->addItem( "Default", QVariant() );
    for ( auto &file: m_audioRecorder->supportedContainers() ) {
        ui->file->addItem( file, QVariant( file ) );
    }

    // 码率
    ui->rate->addItem( "Default", QVariant() );
    for ( auto &rate: m_audioRecorder->supportedAudioSampleRates() ) {
        ui->rate->addItem( QString::number( rate ), QVariant( rate ) );
    }

    connect( ui->start, &QPushButton::clicked, this, &Widget::clickStart );
    connect( ui->stop, &QPushButton::clicked, this, &Widget::clickStop );
    connect( ui->save, &QPushButton::clicked, this, &Widget::clickSave );
}

Widget::~Widget()
{
    delete ui;
}

void Widget::clickStart()
{
    if ( m_audioRecorder->state() == QMediaRecorder::StoppedState ) {
        m_audioRecorder->setAudioInput( boxValue( ui->device ).toString() );

        QAudioEncoderSettings settings;
        settings.setCodec( boxValue( ui->codec ).toString() );
        settings.setSampleRate( boxValue( ui->rate ).toInt() );
        //settings.setBitRate( boxValue(ui->bitrateBox).toInt() );
        //settings.setChannelCount( boxValue(ui->channelsBox).toInt() );
        settings.setQuality( QMultimedia::EncodingQuality::HighQuality );
        settings.setEncodingMode( QMultimedia::EncodingMode::ConstantQualityEncoding );

        QString container = boxValue( ui->file ).toString();

        m_audioRecorder->setEncodingSettings( settings, QVideoEncoderSettings(), container );

        m_audioRecorder->record();
    }
}

void Widget::clickStop()
{
    if ( m_audioRecorder->state() == QMediaRecorder::RecordingState )
        m_audioRecorder->stop();
}

void Widget::clickSave()
{
}


