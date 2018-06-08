//csc /target:library math.cs
namespace Math  
{   
  public class Methods   
  {   
    public Methods()   
    {   
    }   
    public static int CompareI(int a, int b)   
    {   
      if (a>b)  
        return a;  
      else  
        return b;  
    }   
  
    public int CompareII(int a, int b)   
    {   
      if (a>b)  
        return a;  
      else  
        return b;  
    }   
  }   
}