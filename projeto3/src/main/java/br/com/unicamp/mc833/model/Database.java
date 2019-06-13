package main.java.br.com.unicamp.mc833.model;


import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Database {

    private List<Profile> db = new ArrayList<>();

    public Database() {
        db.add(new Profile("gustavoppereira@gmail.com", "Gustavo", "Pereira", "Campinas", "Engenharia", "Fumar Paiero", Collections.singletonList("Fazer porra nenhuma")));
        db.add(new Profile("paulobarros@gmail.com", "Paulo", "Barros", "Campinas", "Ciencia", "Fumar Maconha", Collections.singletonList("Fazer porra nenhuma também")));
    }

    public List<Profile> getEntries() { return db; }

}
