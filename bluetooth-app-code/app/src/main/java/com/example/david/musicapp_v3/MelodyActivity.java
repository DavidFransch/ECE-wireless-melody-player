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
import java.util.ArrayList;

public class MelodyActivity extends AppCompatActivity {
    private static final String TAG = "MelodyActivity";

    BluetoothConnectionService mBluetoothConnection;


    private Button btnA2;
    private Button btnB2;
    private Button btnC2;
    private Button btnD2;
    private Button btnE2;
    private Button btnF2;
    private Button btnG2;

    private Button playBtn2;
    private Button btnBT2;
    private Button melodyBtn;


   //private TextView textView;
    //private ProgressBar progressBar;


    private SeekBar positionBar;
    private SeekBar volumeBar;
    private TextView elapsedTimeLabel;
    private TextView remainingTimeLabel;
    MediaPlayer mp;
    int totalTime;

    //Not tested yet
    ArrayList<String> melody = new ArrayList<String>();
    int progressDuration;
    int progressVolume;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_melody);
        Intent info = getIntent();
        mBluetoothConnection = MainActivity.getBlue();
        //Play Button
        playBtn2 = (Button) findViewById(R.id.playBtn2);
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
        btnA2 = (Button) findViewById(R.id.buttonA2);
        btnA2.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
            @Override
            public void onClick(View view) {
                //Set text
                TextView noteDisp = (TextView) findViewById(R.id.noteDisp);

                //Add letter to string array
                melody.add("A");

                noteDisp.setText("A");
            }
        });

        //Send B parameters
        btnB2 = (Button) findViewById(R.id.buttonB2);
        btnB2.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
            @Override
            public void onClick(View view) {
                //Set text
                TextView noteDisp = (TextView) findViewById(R.id.noteDisp);
                noteDisp.setText("B");

                //Add
                melody.add("B");
            }
        });

        //Send C parameters
        btnC2 = (Button) findViewById(R.id.buttonC2);
        btnC2.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
            @Override
            public void onClick(View view) {
                //Set text
                TextView noteDisp = (TextView) findViewById(R.id.noteDisp);
                noteDisp.setText("C");

                melody.add("C");
            }
        });

        //Send D parameters
        btnD2 = (Button) findViewById(R.id.buttonD2);
        btnD2.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
            @Override
            public void onClick(View view) {
                //Set text
                TextView noteDisp = (TextView) findViewById(R.id.noteDisp);
                noteDisp.setText("D");
                //Add
                melody.add("D");
            }
        });

        //Send E parameters
        btnE2 = (Button) findViewById(R.id.buttonE2);
        btnE2.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
            @Override
            public void onClick(View view) {
                //Set text
                TextView noteDisp = (TextView) findViewById(R.id.noteDisp);
                noteDisp.setText("E");

                melody.add("E");
            }
        });

        //Send F parameters
        btnF2 = (Button) findViewById(R.id.buttonF2);
        btnF2.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
            @Override
            public void onClick(View view) {
                //Set text
                TextView noteDisp = (TextView) findViewById(R.id.noteDisp);
                noteDisp.setText("F");

                melody.add("F");
            }
        });

        //Send G parameters
        btnG2 = (Button) findViewById(R.id.buttonG2);
        btnG2.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
            @Override
            public void onClick(View view) {
                //Set text
                TextView noteDisp = (TextView) findViewById(R.id.noteDisp);
                noteDisp.setText("G");

                melody.add("G");


            }
        });

        //Play button
        playBtn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //playBtnClick();

                String duration = createTimeLabel(progressDuration);
                String volume = Integer.toString(progressVolume);
                String notes = "";
                for(int i = 0; i< melody.size(); i++){
                    notes = notes + melody.get(i);
                }
                //Display melody
                TextView melDisp = (TextView) findViewById(R.id.noteDisp);
                melDisp.setText(notes);

                String param ="true "+ "false " + notes  +"AAA0"+ " " + duration + " " + volume;// 1st true = send & 2nd true = note mode

                //Send params
                byte[] bytes;
                bytes = param.getBytes(Charset.defaultCharset());
                mBluetoothConnection.write(bytes);

            }
        });

        //Setup bluetooth
        btnBT2 = (Button) findViewById(R.id.buttonBT2);
        btnBT2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                openMainActivity();
            }
        });

        //Tune mode switch
        melodyBtn =(Button) findViewById(R.id.melodyBtn);
        melodyBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                openTuneMode();
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
            playBtn2.setBackgroundResource(R.drawable.stop);
        }else{
            //Playing
            mp.pause();
            playBtn2.setBackgroundResource(R.drawable.play);
        }
    }

    public void openMainActivity(){
        Log.d(TAG, "Entered Intent - bluetooth mode");
        Intent intentM = new Intent(this, MainActivity.class);
        startActivity(intentM);
    }

    public void openTuneMode(){
        Log.d(TAG, "Entered Intent - tune mode");
        Intent intentT = new Intent(MelodyActivity.this, TuneActivity.class);
        startActivity(intentT);
    }



//    public void onDestroy(){
//        super.onDestroy();
//        mp.release();
//        handler.removeCallbacks(runnable);
//    }

}
