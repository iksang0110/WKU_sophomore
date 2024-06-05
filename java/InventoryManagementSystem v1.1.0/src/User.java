public class User {
    private int userID;
    private String name;
    private String role;
    private String password;

    public User(int userID, String name, String role, String password) {
        this.userID = userID;
        this.name = name;
        this.role = role;
        this.password = password;
    }

    public User() {}

    public int getUserID() {
        return userID;
    }

    public void setUserID(int userID) {
        this.userID = userID;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getRole() {
        return role;
    }

    public void setRole(String role) {
        this.role = role;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }
}
