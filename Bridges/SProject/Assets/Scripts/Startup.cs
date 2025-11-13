using Strand;
using System;
namespace SProject
{
    public class Startup : Entity
    {
        public static void Init()
        {
            Console.WriteLine("Hello from Startup script!");
        }
    public void OnCreate()
        {
            Console.WriteLine("Hello from Startup script!");
        }
    }
}
