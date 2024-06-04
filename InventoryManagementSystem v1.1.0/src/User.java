public class User {
    private int userID;
    private String name;
    private String role;

    // 필요한 생성자 추가
    public User(int userID, String name, String role) {
        this.userID = userID;
        this.name = name;
        this.role = role;
    }

    // 기본 생성자
    public User() {}

    // Getter 및 Setter 메서드
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
}
