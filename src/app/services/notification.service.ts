import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class NotificationService {
  public granted: boolean = Notification.permission !== 'denied';

  constructor() {
    
   }

  // Check if the browser supports notifications and request permission if needed
  requestPermission() {
    if (!('Notification' in window))
      alert('This browser does not support desktop notification');
    else if (Notification.permission !== 'denied') {
      Notification.requestPermission().then((perm) => {
        if (perm === 'granted')
          this.granted = true;
      });
    }
  }

  // Send a notification if the condition is met
  sendNotification(temp: number) {
    if (!this.granted) return alert('Please enable notifications to receive alerts.');
    // const notification = new Notification('High Temperature Alert!', {
    //   body: `The current temperature is ${temp}°C, which is extremely high.`,
    // });
    navigator.serviceWorker.ready.then((registration) => {
      registration.showNotification('High Temperature Alert!', {
        body: `The current temperature is ${temp}°C, which is extremely high.`
      });
    });  
  }
}
