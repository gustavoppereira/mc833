package main.java.br.com.unicamp.mc833.client;


import main.java.br.com.unicamp.mc833.server.RemoteService;
import main.java.br.com.unicamp.mc833.server.Server;

import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class Client {

    private final String host;
    private final String command;
    private final String query;

    private String experience;

    public Client(final String host, final String command, final String query) {
        this.host = host;
        this.command = command;
        this.query = query;
    }

    public void setExperience(final String experience) {
        this.experience = experience;
    }

    public void loadClient() throws RemoteException, NotBoundException {

        final Registry registry = LocateRegistry.getRegistry(host, Server.PORT);
        final RemoteService remote = (RemoteService) registry.lookup(Server.REGISTRY_NAME);

        executeCommandAndShowResult(remote);
    }

    private void executeCommandAndShowResult(final RemoteService remoteService) throws RemoteException {
        final long initialTime = System.nanoTime();
        switch (command) {
            case "1":
                remoteService.listByCourse(query);
                break;
            case "2":
                remoteService.skillsByCity(query);
                break;
            case "3":
                remoteService.addNewExperienceInProfile(query, experience);
                break;
            case "4":
                remoteService.getExperienceByEmail(query);
                break;
            case "5":
                remoteService.getAllProfiles();
                break;
            case "6":
                remoteService.getProfileByEmail(query);
                break;
        }
        final long finalTime = System.nanoTime();
        System.out.println(finalTime - initialTime);
    }

}
