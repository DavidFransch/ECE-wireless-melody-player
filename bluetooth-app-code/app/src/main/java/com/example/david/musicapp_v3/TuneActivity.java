package com.example.david.musicapp_v3;

import android.content.Intent;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.os.Build;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

import java.nio.charset.Charset;

public class TuneActivity extends AppCompatActivity {
    private static final String TAG = "TuneActivity";

    BluetoothConnectionService mBluetoothConnection;

    private Button btnA;
    private Button btnB;
    private Button btnC;
    private Button btnD;
    private Button btnE;
    private Button btnF;
    private Button btnG;

    private Button playBtn;
    private Button btnBT;
    private Button tuneBtn;

    //private TextView textView;
    //private ProgressBar progressBar;

    private SeekBar positionBar;
    private SeekBar volumeBar;
    private TextView elapsedTimeLabel;
    private TextView remainingTimeLabel;
    MediaPlayer mp;
    int totalTime;

    //Not tested yet
    private static byte[] bytes = new byte[3];
    int progressDuration;
    int progressVolume;

    String note ="";



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tune);
        Intent info = getIntent();
        mBluetoothConnection = MainActivity.getBlue();
        //Play Button
        playBtn = (Button) findViewById(R.id.playBtn);
        elapsedTimeLabel = (TextView) findViewById(R.id.elapsedTimeLabel);
        remainingTimeLabel = (TextView) findViewById(R.id.remainingTimeLabel);

        //Duration/Volume
        // handler = new Handler();
        //Media Player
        mp = MediaPlayer.create(this, R.raw.fkj);
        mp.setLooping(true);
        mp.seekTo(0);
        mp.setVolume(0.5f, 0.5f);
        totalTime = 120000;//mp.getDuration();

        //Position Bar
        positionBar = (SeekBar) findViewById(R.id.positionBar);
        positionBar.setMax(totalTime);
        positionBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                //progressBar.setProgress(progress);
                //textView.setText("" +progress + "%");
                if(fromUser){
                    progressDuration = progress; //Save and send duration
                    mp.seekTo(progress);
                    positionBar.setProgress(progress);
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        //Volume Bar
        volumeBar = (SeekBar) findViewById(R.id.volumeBar);
        volumeBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
             @Override
             public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                 progressVolume = progress; //Save and send volume
                 float volumeNum = progress/100f;
                 mp.setVolume(volumeNum, volumeNum);
             }

             @Override
             public void onStartTrackingTouch(SeekBar seekBar) {

             }

             @Override
             public void onStopTrackingTouch(SeekBar seekBar) {

             }
         }
        );

        //Thread (Update positionBar & timeLabel)
        new Thread(new Runnable() {
            @Override
            public void run() {
                while(mp!=null){
                    try{
                        Message msg = new Message();
                        msg.what = mp.getCurrentPosition();
                        handler.sendMessage(msg);
                        Thread.sleep(1000);
                    }catch (InterruptedException e){}
                }
            }
        }).start();

        //Send A parameters
        btnA = (Button) findViewById(R.id.buttonA);
        btnA.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
            @Override
            public void onClick(View view) {
                //Set text
                TextView noteDisp = (TextView) findViewById(R.id.noteDisp);
                noteDisp.setText("A");
                note = "A";

            }
        });

        //Send B parameters
        btnB = (Button) findViewById(R.id.buttonB);
        btnB.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
            @Override
            public void onClick(View view) {
                //Set text
                TextView noteDisp = (TextView) findViewById(R.id.noteDisp);
                noteDisp.setText("B");
                note = "B";
            }
        });

        //Send C parameters
        btnC = (Button) findViewById(R.id.buttonC);
        btnC.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
            @Override
            public void onClick(View view) {
                //Set text
                TextView noteDisp = (TextView) findViewById(R.id.noteDisp);
                noteDisp.setText("C");
                note = "C";
            }
        });

        //Send D parameters
        btnD = (Button) findViewById(R.id.buttonD);
        btnD.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
            @Override
            public void onClick(View view) {
                //Set text
                TextView noteDisp = (TextView) findViewById(R.id.noteDisp);
                noteDisp.setText("D");
                note = "D";
            }
        });

        //Send E parameters
        btnE = (Button) findViewById(R.id.buttonE);
        btnE.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
            @Override
            public void onClick(View view) {
                //Set text
                TextView noteDisp = (TextView) findViewById(R.id.noteDisp);
                noteDisp.setText("E");
                note = "E";
            }
        });

        //Send F parameters
        btnF = (Button) findViewById(R.id.buttonF);
        btnF.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
            @Override
            public void onClick(View view) {
                //Set text
                TextView noteDisp = (TextView) findViewById(R.id.noteDisp);
                noteDisp.setText("F");
                note = "F";
            }
        });

        //Send G parameters
        btnG = (Button) findViewById(R.id.buttonG);
        btnG.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
            @Override
            public void onClick(View view) {
                //Set text
                TextView noteDisp = (TextView) findViewById(R.id.noteDisp);
                noteDisp.setText("G");
                note = "G";

            }
        });

        //Play button
        playBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //String duration = Integer.toString(progressDuration);
                String duration = createTimeLabel(progressDuration);
                String volume = Integer.toString(progressVolume);
                String param = "true " + "true " + note + " " + duration + " " + volume; // 1st true = send & 2nd true = note mode

                //Send params
                byte[] bytes;
                bytes = param.getBytes(Charset.defaultCharset());
                mBluetoothConnection.write(bytes);
            }
        });

        //Setup bluetooth
        btnBT = (Button) findViewById(R.id.buttonBT);
        btnBT.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                openMainActivity();
            }
        });

        //Melody mode switch
        tuneBtn =(Button) findViewById(R.id.tuneBtn);
        tuneBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                openMelodyMode();
            }
        });

    }

    private Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            //super.handleMessage(msg);
            int currentPosition = msg.what;
            //Update positionBar
            positionBar.setProgress(currentPosition);

            //Update labels
            String elapsedTime = createTimeLabel(currentPosition);
            elapsedTimeLabel.setText(elapsedTime);

            String remainingTime = createTimeLabel(totalTime=currentPosition);
            remainingTimeLabel.setText("- "+ remainingTime);
        }
    };

    public String createTimeLabel(int time){
        String timeLabel = "";
        int min = time/1000/60;
        int sec = time/1000%60;

        timeLabel = min + ":";
        if(sec<10) timeLabel += "0";
        timeLabel+= sec;

        return timeLabel;
    }

    public void playBtnClick(){
        if(!mp.isPlaying()){
            //Stopping
            mp.start();
            playBtn.setBackgroundResource(R.drawable.stop);
        }else{
            //Playing
            mp.pause();
            playBtn.setBackgroundResource(R.drawable.play);
        }
    }

    public void openMainActivity(){
        Log.d(TAG, "Entered Intent - bluetooth mode");
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
    }

    public void openMelodyMode(){
        Log.d(TAG, "Entered Intent - melody mode");
        Intent intent = new Intent(this, MelodyActivity.class);
        startActivity(intent);
    }



//    public void onDestroy(){
//        super.onDestroy();
//        mp.release();
//        handler.removeCallbacks(runnable);
//    }

}
