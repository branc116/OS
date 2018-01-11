using System;
using System.Linq;
using System.Threading.Tasks;

namespace labosi4
{
    class Program
    {
        static async Task Main(string[] args)
        {
            Console.WriteLine("Hello World!");
            
            var a = new MyThreadManager(
                ManagerType.Priority,
                from i in Enumerable.Range(0, 10) 
                select new MyThread(i, 5, $"{i%2}"))
            .Start();
            await a;
        }
        static string randString() {
            var rand = new Random();
            return new string((from charac in Enumerable.Range(0, 10)
                                select (char)('A' + rand.Next(0, 25))).ToArray());
        }
    }
}
