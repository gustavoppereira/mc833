package main.java.br.com.unicamp.mc833.server;


import main.java.br.com.unicamp.mc833.model.Database;
import main.java.br.com.unicamp.mc833.model.Profile;

import java.util.List;
import java.util.stream.Collectors;

public class RemoteServiceImpl implements RemoteService {

    private final List<Profile> database = new Database().getEntries();

    private final ServerLogDispatcher logDispatcher;

    RemoteServiceImpl(final ServerLogDispatcher serverLogDispatcher) {
        this.logDispatcher = serverLogDispatcher;
    }

    @Override
    public List<Profile> listByCourse(String course) {
        final long startTime = System.nanoTime();
         final List<Profile> result = database.stream().filter(profile -> profile.getCourse().equalsIgnoreCase(course))
                .collect(Collectors.toList());
        final long endTime = System.nanoTime();

        logDispatcher.dispatch(String.valueOf(endTime - startTime));
        return result;
    }

    @Override
    public List<String> skillsByCity(String city) {
        final long startTime = System.nanoTime();
        final List<String> result = database.stream().filter(profile -> profile.getCity().equalsIgnoreCase(city))
                .map(Profile::getSkill).collect(Collectors.toList());
        final long endTime = System.nanoTime();

        logDispatcher.dispatch(String.valueOf(endTime - startTime));
        return result;
    }

    @Override
    public void addNewExperienceInProfile(String email, String experience) {
        final long startTime = System.nanoTime();
        database.forEach(profile -> {
            if (profile.getEmail().equalsIgnoreCase(email)) {
                profile.getExperience().add(experience);
            }
        });
        final long endTime = System.nanoTime();
        logDispatcher.dispatch(String.valueOf(endTime - startTime));
    }

    @SuppressWarnings("ConstantConditions")
    @Override
    public List<String> getExperienceByEmail(String email) {
        final long startTime = System.nanoTime();
        final List<String> result = database.stream().filter(profile -> profile.getEmail().equalsIgnoreCase(email)).findAny()
                .orElse(null).getExperience();
        final long endTime = System.nanoTime();

        logDispatcher.dispatch(String.valueOf(endTime - startTime));
        return result;
    }

    @Override
    public List<Profile> getAllProfiles() {
        return database;
    }

    @Override
    public Profile getProfileByEmail(String email) {
        final long startTime = System.nanoTime();
        final Profile result = database.stream().filter(profile -> profile.getEmail().equalsIgnoreCase(email))
                .findAny().orElse(null);
        final long endTime = System.nanoTime();

        logDispatcher.dispatch(String.valueOf(endTime - startTime));
        return result;
    }
}
