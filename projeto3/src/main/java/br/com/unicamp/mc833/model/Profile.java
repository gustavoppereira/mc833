package main.java.br.com.unicamp.mc833.model;


import java.io.Serializable;
import java.util.List;

public class Profile implements Serializable {

    private String email;
    private String firstName;
    private String lastName;
    private String city;
    private String course;
    private String skill;

    public Profile(String email, String firstName, String lastName, String city, String course, String skill, List<String> experiences) {
        this.email = email;
        this.firstName = firstName;
        this.lastName = lastName;
        this.city = city;
        this.course = course;
        this.skill = skill;
        this.experiences = experiences;
    }

    private List<String> experiences;

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getFirstName() {
        return firstName;
    }

    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }

    public String getLastName() {
        return lastName;
    }

    public void setLastName(String lastName) {
        this.lastName = lastName;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }

    public String getCourse() {
        return course;
    }

    public void setCourse(String course) {
        this.course = course;
    }

    public String getSkill() {
        return skill;
    }

    public void setSkill(String skill) {
        this.skill = skill;
    }

    public List<String> getExperience() {
        return experiences;
    }

    public void setExperiences(List<String> experiences) {
        this.experiences = experiences;
    }

}
