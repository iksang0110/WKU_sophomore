import java.util.List;
import java.util.ArrayList;

public class Utils {
    public static List<Member> searchMembers(List<Member> members, String searchField, String searchValue) {
        List<Member> results = new ArrayList<>();
        for (Member member : members) {
            switch (searchField.toLowerCase()) {
                case "name":
                    if (member.getName().equalsIgnoreCase(searchValue)) results.add(member);
                    break;
                case "studentid":
                    if (member.getStudentId().equalsIgnoreCase(searchValue)) results.add(member);
                    break;
                case "grade":
                    if (Integer.toString(member.getGrade()).equals(searchValue)) results.add(member);
                    break;
            }
        }
        return results;
    }

    public static void displayResults(List<Member> results) {
        for (Member result : results) {
            System.out.println(result);
        }
    }
}