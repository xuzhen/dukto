package com.github.xuzhen.dukto;

import org.qtproject.qt.android.bindings.QtActivity;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import java.util.ArrayList;

public class MainActivity extends QtActivity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        handleIntent(getIntent());
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);
        handleIntent(intent);
    }

    private void handleIntent(Intent intent) {
        if (intent == null) return;
        
        String action = intent.getAction();
        String type = intent.getType();

        if (type == null) {
            return;
        }

        if (Intent.ACTION_SEND.equals(action)) {
            if ("text/plain".equals(type)) {
                String sharedText = intent.getStringExtra(Intent.EXTRA_TEXT);
                if (sharedText != null) {
                    onTextShared(sharedText);
                }
            } else {
                Uri uri = null;
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                    uri = intent.getParcelableExtra(Intent.EXTRA_STREAM, Uri.class);
                } else {
                    @SuppressWarnings("deprecation")
                    Uri legacyUri = intent.getParcelableExtra(Intent.EXTRA_STREAM);
                    uri = legacyUri;
                }
                if (uri != null) {
                    onFileShared(uri.toString());
                }
            }
        } else if (Intent.ACTION_SEND_MULTIPLE.equals(action)) {
            ArrayList<Uri> uris = null;
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                uris = intent.getParcelableArrayListExtra(Intent.EXTRA_STREAM, Uri.class);
            } else {
                @SuppressWarnings("deprecation")
                ArrayList<Uri> legacyUris = intent.getParcelableArrayListExtra(Intent.EXTRA_STREAM);
                uris = legacyUris;
            }
            if (uris != null && !uris.isEmpty()) {
                String[] uriArray = new String[uris.size()];
                for (int i = 0; i < uris.size(); i++) {
                    uriArray[i] = uris.get(i).toString();
                }
                onMultipleFilesShared(uriArray);
            }
        }
    }

    public native void onFileShared(String uriString);
    public native void onMultipleFilesShared(String[] uriArray);
    public native void onTextShared(String text);
}

