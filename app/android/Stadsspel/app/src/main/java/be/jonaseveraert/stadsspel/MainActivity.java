package be.jonaseveraert.stadsspel;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import be.jonaseveraert.stadsspel.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'stadsspel' library on application startup.
    static {
        System.loadLibrary("stadsspel");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'stadsspel' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}