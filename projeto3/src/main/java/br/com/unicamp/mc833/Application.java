package main.java.br.com.unicamp.mc833;


import main.java.br.com.unicamp.mc833.client.Client;
import main.java.br.com.unicamp.mc833.server.Server;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;

public class Application {

    private static final String CLIENT = "client";
    private static final String SERVER = "server";

    public static void main(String[] args) {
        final String side = args[0];

        try {
            switch (side) {
                case CLIENT:
                    executeClient();
                    break;

                case SERVER:
                    executeServer();
                    break;
            }
        } catch (IOException exception) {
            exception.printStackTrace();
        }
    }

    private static void executeClient() throws IOException {
        final BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        System.out.println("Insert host:");
        final String host = reader.readLine();
        System.out.println("Insert command:");
        final String command = reader.readLine();
        System.out.println("Insert query:");
        final String query = reader.readLine();

        final Client client = new Client(host, command, query);
        if ("3".equalsIgnoreCase(command)) {
            System.out.println("Insert a new experience:");
            client.setExperience(reader.readLine());
        }
        try {
            client.loadClient();
        } catch (RemoteException | NotBoundException e) {
            e.printStackTrace();
        }
    }

    private static void executeServer() {
        final Server server = new Server();
        try {
            server.initializeServer();
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

}
