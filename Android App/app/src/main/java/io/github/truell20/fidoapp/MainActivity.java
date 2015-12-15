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

/* The primary activity of the app. Allows a user to connect to the Fido robot
 *
 */

public class MainActivity extends AppCompatActivity {
    private final static int REQUEST_ENABLE_BT = 1;
    private final static int REQUEST_ENABLE_DISCOVER = 2;
    private final static String FIDO_BLUETOOTH_NAME = "Fido";
    private final UUID FIDO_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");


    private BluetoothDevice fido = null;
    private BluetoothAdapter adapter = null;
    private BluetoothSocket socket = null;
    private InputStream in = null;
    private OutputStream out = null;


    // Called when the activity is initiated.
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        enableDiscoverability();

        // Setup a listener for the connect button
        // When the user presses the button, look for Fido and attempt to connect to Fido
        ((Button) findViewById(R.id.connectButton)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                findFido();
                connectToFido();
            }
        });
    }

    // Get a the default Bluetooth adapter. If the user has not enable bluetooth, ask them to enable bluetooth
    public BluetoothAdapter getBluetoothAdapter() {
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        if (!adapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            return null;
        }
        return adapter;
    }

    public void enableDiscoverability() {
        Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
        discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 3600);
        startActivityForResult(discoverableIntent, REQUEST_ENABLE_DISCOVER);
    }

    // Looks for Fido among paired devices and sets the instance variable fido to the found Bluetooth device
    // If Fido cannot be found, the user is alerted.
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

    // If the instance variable fido is set, attempts to connect to Fido over a bluetooth socket.
    // Initializes the out and in ivars, which contain the OutputStream and InputStream of the socket respectively.
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

    // Sends a String to Fido
    private void sendString(String message) {
        try {
            byte[] buffer = message.getBytes();
            out.write(buffer);
        } catch(Exception e) {}
    }

    // Gets a String from Fido
    private String getStringFromFido(int size) {
        try {
            byte[] buffer = new byte[size];
            in.read(buffer);
            return new String(buffer);
        } catch(Exception e) {
            return null;
        }
    }

    // Is called when a request for bluetooth finishes
    // If the user didnt authorize bluetooth use, the app requests bluetooth access again.
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(requestCode == REQUEST_ENABLE_BT && resultCode != REQUEST_ENABLE_BT) {
            adapter = getBluetoothAdapter();
        }
        if(requestCode == REQUEST_ENABLE_DISCOVER && resultCode == this.RESULT_CANCELED) {
            enableDiscoverability();
        }
    }
}
