package controller;

import view.*;
import javax.swing.*;

public class Main {

    public static void main(String[] args){

        SwingUtilities.invokeLater(() -> {
            try {
                UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            } catch (Exception e) {
                e.printStackTrace();
            }
            MainView view = new MainView();
            Listener l = new Listener(view);
            view.addListeners(l);
            view.makeWindowVisible();
        });
    }

}
