package com.example.david.musicapp_v3;


import android.annotation.TargetApi;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.Charset;
import java.util.UUID;


public class BluetoothConnectionService {
    private static final String TAG = "BluetoothConnectionServ";

    private static final String appName = "MYAPP";

    private static final UUID MY_UUID_INSECURE =
            UUID.fromString("8ce255c0-200a-11e0-ac64-0800200c9a66");//pi = 8209df2c-97f5-11e8-b022-2fb3d7663398

    private final BluetoothAdapter mBluetoothAdapter;

    Context mContext;

    private AcceptThread mInsecureAcceptThread;
    private ConnectThread mConnectThread;
    private BluetoothDevice mmDevice;
    private UUID deviceUUID;
    ProgressDialog mProgressDialog;

    private ConnectedThread mConnectedThread;

    //constructor
    @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD_MR1)
    //@RequiresApi(api = Build.VERSION_CODES.ECLAIR)
    public BluetoothConnectionService(Context context){
        mContext = context;
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        start();
    }

    /*
    This thread runs whilst listening for incoming connections. Behaves
    like server-side client. Runs until connection is accpeted or until cancelled.
    #######################################ACCEPT##################################################
     */
    private class AcceptThread extends Thread{
        private final BluetoothServerSocket mmServerSocket;

        @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD_MR1)
        public AcceptThread(){
            BluetoothServerSocket tmp = null;
            //Create a new listening server socket
            try{
                tmp = mBluetoothAdapter.listenUsingInsecureRfcommWithServiceRecord(appName, MY_UUID_INSECURE);
                Log.d(TAG, "AcceptThread: Setting up Server using" + MY_UUID_INSECURE);
            }
            catch(IOException e){

            }
            mmServerSocket = tmp;

        }
        @RequiresApi(api = Build.VERSION_CODES.ECLAIR)
        public void run(){
            Log.d(TAG, "run: AcceptThread Running");
            BluetoothSocket socket = null;
            /*
            This is a blocking call and will only return on a
            successful connection or an exception
             */

            try{
                Log.d(TAG, "run: RFCOM server socket start ...");
                socket = mmServerSocket.accept();
                Log.d(TAG, "run: RFCOM server accepted connection ...");
            }
            catch(IOException e){
                Log.e(TAG, "AcceptThread: IOException" + e.getMessage() );
            }
            if(socket!=null){
                connected(socket, mmDevice);
            }
            Log.i(TAG, "END mAcceptThread");

        }
        public void cancel(){
            Log.d(TAG, "cancel: Cancelling AcceptThread");
            try{
                mmServerSocket.close();
            }catch(IOException e){
                Log.e(TAG, "cancel: Close of AcceptThread ServerSocket failed." + e.getMessage());
            }

        }
    }
    /*
    This thread runs while attempting to make an outgoing connection with a device.
    It runs straight through; the connection either succeeds or fails.
    ##############################################CONNECT#################################################
     */
    private class ConnectThread extends Thread{
        private BluetoothSocket mmSocket;
        public ConnectThread(BluetoothDevice device, UUID uuid){
            Log.d(TAG,  "ConnectThread: started.");
            mmDevice = device;
            deviceUUID = uuid;
        }

        @RequiresApi(api = Build.VERSION_CODES.ECLAIR)
        public void run(){
            BluetoothSocket tmp = null;
            Log.i(TAG, "RUN mConnectThread");

            //Get a BluetoothSocket for a connection with the given BluetoothDevice
            try{
                tmp = mmDevice.createRfcommSocketToServiceRecord(deviceUUID);
            }catch (IOException e){
                Log.e(TAG, "ConnectThread: Could not create InsecureRFcommSocket" + e.getMessage());
            }
            mmSocket = tmp;
            //Always cancel discovery because it will slow down a connection
            mBluetoothAdapter.cancelDiscovery();
            try{
                mmSocket.connect();

                Log.d(TAG, "run: ConnectThread connected");
            }
            catch (IOException e){
                //Close socket
                try{
                    mmSocket.close();
                    Log.d(TAG, "run: Closed Socket");
                }catch (IOException e1){
                    Log.e(TAG, "mConnectThread: run: Unable to close connection in socket"+ e1.getMessage());
                }
                Log.d(TAG, "run: ConnectThread: Could not connect to UUID:" + MY_UUID_INSECURE);
            }

            connected(mmSocket, mmDevice);

        }
        public void cancel(){
            try{
                Log.d(TAG, "cancel:Closing Client Socket");
                mmSocket.close();
            } catch (IOException e){
                Log.e(TAG, "cancel:close() of mmSocket in ConnectThread failed."+ e.getMessage());
            }
        }

    }

    /*
    #########################################START##########################################################+
    Specifically start AcceptThread to begin a session in listening (server) mode. Called by the Acticity onResume()
     */
    @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD_MR1)
    public synchronized void start(){
        Log.d(TAG, "start");
        //Cancel any thread attempting to make a connection
        if(mConnectThread !=null){
            mConnectThread.cancel();
            mConnectThread = null;
        }
        if(mInsecureAcceptThread == null){
            mInsecureAcceptThread = new AcceptThread();
            mInsecureAcceptThread.start();
        }
    }
    /*
    AcceptThread starts and sits waiting for a connection.
    Then ConnectThread starts and attempts to make a connection with the other devices AcceptThread
     */
    public void startClient(BluetoothDevice device, UUID uuid){
        Log.d(TAG, "startClient: Started.");

        //initprogress dialog
        mProgressDialog = ProgressDialog.show(mContext, "Connecting Bluetooth", "Please Wait...", true);
        mConnectThread = new ConnectThread(device, uuid);
        mConnectThread.start();
    }

    private class ConnectedThread extends Thread{
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        @RequiresApi(api = Build.VERSION_CODES.ECLAIR)
        public ConnectedThread(BluetoothSocket socket){
            Log.d(TAG, "ConnectedThread: Starting");

            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            //dismiss the progressdialog when connection is established
            try{
                mProgressDialog.dismiss();
            }catch (NullPointerException e){
                e.printStackTrace();
            }

            try{
                tmpIn = mmSocket.getInputStream();
                tmpOut = mmSocket.getOutputStream();
            }catch (IOException e){
                e.printStackTrace();
            }
            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run(){
            byte[] buffer = new byte[1024]; //buffer store for the stream
            int bytes;//bytes returned from read()

            //keep listening to InputStream until and exception occurs
            while(true){
                try{
                    bytes = mmInStream.read(buffer);
                    String incomingMessage = new String(buffer, 0, bytes);
                    Log.d(TAG, "InputStream: "+ incomingMessage);
                }catch (IOException e){
                    Log.e(TAG, "write: Error reading inputStream." + e.getMessage());
                    break;
                }

            }
        }
        //Call this from main activity to send data to remote device
        @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
        public void write(byte[] bytes){
            String text = new String(bytes, Charset.defaultCharset());
            Log.d(TAG, "write: Writing to outputstream:" + text);
            try {
                mmOutStream.write(bytes);
            } catch (IOException e) {
                Log.e(TAG, "write: Error writing to outputStream." + e.getMessage());
            }
        }

        public void cancel(){
            try{
                mmSocket.close();
            }catch(IOException e){ }
        }
    }
    @RequiresApi(api = Build.VERSION_CODES.ECLAIR)
    private void connected(BluetoothSocket mmSocket, BluetoothDevice mmDevice) {
        Log.d(TAG, "connected: Starting");
        mConnectedThread = new ConnectedThread(mmSocket);
        mConnectedThread.start();
    }
    /*
        Write to the ConnectedThread in an unsynchronized manner
        @param out The bytes to write
        @see ConnectedThread.write(byte[])
     */
    @RequiresApi(api = Build.VERSION_CODES.GINGERBREAD)
    public void write(byte[] out){
        //Create temporary object
        ConnectedThread r;
        //Synchronize a copy of the ConnectedThread
        Log.d(TAG, "write: Write Called.");
        mConnectedThread.write(out);

    }
}
