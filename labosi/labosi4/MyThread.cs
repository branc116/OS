using System;
using NLog;
namespace labosi4
{
    public class MyThread {
        private static Logger _logger = LogManager.GetCurrentClassLogger();
        private const MyThreadTypes DEF = MyThreadTypes.TimeShare;
        public int Id { get; set; }
        public int WorkLeft { get; set; }
        public string Priority { get; set; }
        public DateTime Added { get; set; }
        public MyThreadTypes ThreadType { get; set; }
        public static MyThread NOPE = new MyThread(0, 1, "NOPE", MyThreadTypes.TimeShare);
        public MyThread(int id, int workLeft, string priority)
        {
            _logger.Trace($"Creating Thread(id = {id}, startingWork = {workLeft}, priority = {priority}, threadType = {DEF.ToString()})");
            Id = id;
            WorkLeft = workLeft;
            Priority = priority;
            this.Added = DateTime.Now;
            this.ThreadType = DEF;
        }
        public MyThread(int id, int workLeft, string priority, MyThreadTypes threadType) {
            _logger.Trace($"Creating Thread(id = {id}, startingWork = {workLeft}, priority = {priority}, threadType = {threadType.ToString()})");
            this.Id = id;
            this.WorkLeft = workLeft;
            this.Priority = priority;
            this.Added = DateTime.Now;
            this.ThreadType = threadType;
        }
        public override string ToString() {
            return $"id = {this.Id}, startingWork = {this.WorkLeft}, priority = {this.Priority}, threadType = {this.ThreadType.ToString()}";
        }
    }
}