package main.java.br.com.unicamp.mc833.server;

import main.java.br.com.unicamp.mc833.model.Profile;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;

public interface RemoteService extends Remote {

    List<Profile> listByCourse(final String course) throws RemoteException;

    List<String> skillsByCity(final String city) throws RemoteException;

    void addNewExperienceInProfile(final String email, final String experience) throws RemoteException;

    List<String> getExperienceByEmail(final String email) throws RemoteException;

    List<Profile> getAllProfiles() throws RemoteException;

    Profile getProfileByEmail(final String email) throws RemoteException;

}
