package io.github.truell20.fidoapp;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    private final static int REQUEST_ENABLE_BT = 1;
    private final static String FIDO_BLUETOOTH_NAME = "Fido";
    private final UUID FIDO_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");


    private BluetoothDevice fido = null;
    private BluetoothAdapter adapter = null;
    private BluetoothSocket socket = null;
    private InputStream in = null;
    private OutputStream out = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        adapter = getBluetoothAdapter();

        ((Button) findViewById(R.id.connectButton)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                findFido();
                connectToFido();
            }
        });
    }

    public BluetoothAdapter getBluetoothAdapter() {
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        if (!adapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            return null;
        }
        return adapter;
    }

    public void findFido() {
        if (adapter == null || fido != null) return;

        Set<BluetoothDevice> pairedDevices = adapter.getBondedDevices();

        if (pairedDevices.size() <= 0) {
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setMessage("No paired bluetooth devices").setTitle("Error");
            AlertDialog dialog = builder.create();
            dialog.show();
            return;
        }

        for (BluetoothDevice device : pairedDevices) {
            if (device.getName().equals(FIDO_BLUETOOTH_NAME)) {
                fido = device;
                break;
            }
        }

        if (fido == null) {
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setMessage("Could not find Fido among paired devices").setTitle("Error");
            AlertDialog dialog = builder.create();
            dialog.show();
        } else {
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setMessage("Connected!").setTitle("Success");
            AlertDialog dialog = builder.create();
            dialog.show();
        }
    }

    private void connectToFido() {
        if(fido == null) return;

        try {
            socket = fido.createRfcommSocketToServiceRecord(FIDO_UUID);
            socket.connect();

            in = socket.getInputStream();
            out = socket.getOutputStream();
        } catch (IOException e) {
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setMessage("Error conneecting to Fido").setTitle("Error");
            AlertDialog dialog = builder.create();
            dialog.show();

            socket = null;
            in = null;
            socket = null;
        }
    }

    private void sendToFido(String message) {
        try {
            byte[] buffer = message.getBytes();
            out.write(buffer);
        } catch(Exception e) {}
    }

    private String getFromFido(int size) {
        try {
            byte[] buffer = new byte[size];
            in.read(buffer);
            return new String(buffer);
        } catch(Exception e) {
            return null;
        }
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(requestCode == REQUEST_ENABLE_BT && resultCode != REQUEST_ENABLE_BT) {
            adapter = getBluetoothAdapter();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    private class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            // Get the input and output streams, using temp objects because
            // member streams are final
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run() {
            byte[] buffer = new byte[1024];  // buffer store for the stream
            int bytes; // bytes returned from read()

            // Keep listening to the InputStream until an exception occurs
            while (true) {
                try {
                    // Read from the InputStream
                    bytes = mmInStream.read(buffer);
                    mmOutStream.write(2);
                } catch (IOException e) {
                    break;
                }
            }
        }

        public void write(byte[] bytes) {
            try {
                mmOutStream.write(bytes);
            } catch (IOException e) { }
        }

        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) { }
        }
    }
}
