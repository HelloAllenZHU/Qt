#include <QUrl>
#include <QAudioProbe>
#include <QAudioRecorder>
#include "Widget.h"
#include "ui_widget.h"

// 创建空的wav文件
// ffmpeg -f lavfi -i anullsrc -t 01:00:00 silence.wav
// ffmpeg -f lavfi -i anullsrc=r=44100:cl=mono -t "00:01:00" -q:a 9 -acodec libmp3lame "c:/1.wav"

// 将clip音频合成到空的wav里面
// ffmpeg -i silence.wav -i clip1.wav -i clip2.wav -filter_complex "aevalsrc=0:d=10[s1];aevalsrc=0:d=20[s2];[s1][1:a]concat=n=2:v=0:a=1[ac1];[s2][2:a]concat=n=2:v=0:a=1[ac2];[0:a][ac1][ac2]amix=3[aout]" -map [aout] out.wav
// ffmpeg -i <blank audio> -i < audio file 2> -filter_complex "aevalsrc=0:d= <time> [s1];[s1][1:a]concat=n=2:v=0:a=1[aout]" -c:v copy -map 0:v -map [aout] <output file>

// 将上面的wav合成到背景wav里面
// ffmpeg -i <output from silent and short clip> -i <original long clip> -filter_complex "amix=inputs=2:duration=longest:dropout_transition=0, volume=2" <output audio file>

// wav转aac
// ffmpeg -i input.wav -codec:a aac output.aac

// 参考xxx
// ffmpeg -i video.mp4 -ss 00:04:00 -i audio.mp3 -c copy -shortest output.mkv
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
    connect(m_probe, &QAudioProbe::audioBufferProbed,
            this, &Widget::processBuffer);
    m_probe->setSource(m_audioRecorder);

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
        // 编码设置
        QAudioEncoderSettings settings;
        settings.setCodec( boxValue( ui->codec ).toString() );
        settings.setSampleRate( boxValue( ui->rate ).toInt() );
        settings.setBitRate( 64000 );
        settings.setChannelCount( 2 );
        settings.setQuality( QMultimedia::EncodingQuality::HighQuality );
        settings.setEncodingMode( QMultimedia::EncodingMode::ConstantQualityEncoding );

        // 文件容器设置
        QString container = boxValue( ui->file ).toString();
        m_audioRecorder->setEncodingSettings( settings, QVideoEncoderSettings(), container );
        m_audioRecorder->setAudioInput( boxValue( ui->device ).toString() );

        // 保存到文件
        m_audioRecorder->setOutputLocation( QUrl::fromLocalFile( "c:/record" ) );

        // 开始录制
        m_audioRecorder->record();

        // 更新UI状态
        ui->start->setEnabled( false );
        ui->stop->setEnabled( true );
    }
}

void Widget::clickStop()
{
    if ( m_audioRecorder->state() == QMediaRecorder::RecordingState ) {
        m_audioRecorder->stop();
        ui->start->setEnabled( true );
        ui->stop->setEnabled( false );
    }
}

void Widget::clickSave()
{

}

void Widget::processBuffer( const QAudioBuffer& buffer )
{
    ;
}


