package main.java.br.com.unicamp.mc833.server;


import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

public class Server implements ServerLogDispatcher {

    public static final String REGISTRY_NAME = "RemoteService";
    public static final int PORT = 8001;

    private final RemoteService remoteService = new RemoteServiceImpl(this);

    public void initializeServer() throws RemoteException {
        final RemoteService stub = (RemoteService) UnicastRemoteObject.exportObject(remoteService, PORT);
        createRegistry(stub);

        System.out.println("Server initialized");
    }

    private void createRegistry(final RemoteService stub) throws RemoteException {
        final Registry registry = LocateRegistry.createRegistry(PORT);
        registry.rebind(REGISTRY_NAME, stub);
    }

    @Override
    public void dispatch(String log) {
        System.out.println(log);
    }
}