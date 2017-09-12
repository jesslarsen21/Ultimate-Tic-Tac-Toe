package edu.dixie.cit.ultimatetic_tac_toe;

import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;


public class GameActivity extends ActionBarActivity {
    private static enum Players { PLAYER_X, PLAYER_O };

    private final String LOG_TAG = GameActivity.class.getSimpleName();

    private Players turn = Players.PLAYER_X;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);


        if (findViewById(R.id.fragment_container) != null) {

            // However, if we're being restored from a previous state,
            // then we don't need to do anything and should return or else
            // we could end up with overlapping fragments.
            if (savedInstanceState != null) {
                return;
            }

            // Create a new Fragment to be placed in the activity layout
            BoardFragment boardFragment = new BoardFragment();

            // Add the fragment to the 'fragment_container' FrameLayout
            getSupportFragmentManager().beginTransaction().add(R.id.fragment_container, boardFragment).commit();
        }
    }

    public void boardClick(View v) {
        switch(turn) {
            case PLAYER_X:
                turn = Players.PLAYER_O;
                break;
            case PLAYER_O:
                turn = Players.PLAYER_X;
                break;
            default:
                Log.e(LOG_TAG, "Unknown Turn?"+turn);
        }
    }

    public void subBoardClick(View v) {
        SubBoardFragment newFragment = new SubBoardFragment();

        android.support.v4.app.FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();

        transaction.replace(R.id.fragment_container, newFragment);
        transaction.addToBackStack(null);

        transaction.commit();
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_game, menu);
        super.onCreateOptionsMenu(menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
