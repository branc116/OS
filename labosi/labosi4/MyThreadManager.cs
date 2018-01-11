using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using NLog;

namespace labosi4 
{
    public class MyThreadManager {
        private static Logger _logger = LogManager.GetCurrentClassLogger();
        public List<MyThread> Threads { get; set; }
        public ManagerType ManagerType { get; set; } 
        public bool Pause { get; set; }
        private bool _running;
        private const int QuantTimeMs = 1000;
        public MyThreadManager(ManagerType managerType, IEnumerable<MyThread> threads = null) 
        {
            _logger.Trace($"Creating new ThreadManager(ManagerType = {managerType.ToString()}");
            this.ManagerType = managerType;
            if (threads != null) 
                this.Threads= new List<MyThread>(threads);
            else
                this.Threads = new List<MyThread>();
        }
        private async Task Tick() {
            var activeThread  = NextThread().First();
            do {
                _logger.Debug($"Working on {activeThread}");
                await Task.Delay(QuantTimeMs);
                activeThread.WorkLeft--;
                LogCurrnetState();
            }while(activeThread.WorkLeft > 0 && activeThread.ThreadType == MyThreadTypes.Greed);

            if (activeThread.WorkLeft <= 0) {
                _logger.Debug($"Thread {activeThread} is finished!");
                Threads.RemoveAll(i => i.Id == activeThread.Id);
            }

            activeThread.Added = DateTime.Now;
        }
        public async Task Start() {
            _running = true;
            while(_running) {
                if (!Pause) {
                    await Tick();
                }
                else {
                    _logger.Trace("Manager is paused");
                    await Task.Delay(QuantTimeMs);
                }
            }
        }
        public void Stop() {
            _running = false;
        }
        private void LogCurrnetState() {
            _logger.Debug(this);
            foreach(var thread in NextThread()) {
                _logger.Debug(thread);
            }
        }
        private IEnumerable<MyThread> NextThread() {
            if (this.Threads == null || !this.Threads.Any()) {
                _logger.Warn($"There are no threads to work on :(");
                return new [] { MyThread.NOPE};
            }
            switch(ManagerType) {
                case ManagerType.Fifo:
                    return Threads
                    .Where(j => j.Added == Threads
                        .Min(i => i.Added))
                    .OrderBy(i => i.Priority);
                case ManagerType.Lifo:
                    return Threads
                    .Where(j => j.Added == Threads
                        .Max(i => i.Added))
                    .OrderBy(i => i.Priority);
                case ManagerType.Priority:
                    return Threads
                    .Where(j => j.Priority == Threads
                        .Max(i => i.Priority))
                    .OrderBy(i => i.Added);
            }
            return new []{ MyThread.NOPE };
        }
        public override string ToString() {
            return $"Running = {this._running}, Type = {this.ManagerType}, Is Paused = {this.Pause}, Number of threads = {this.Threads.Count}";
        }
    }
}