import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class NotificationService {
  granted: boolean = false;

  constructor() { }

  // Check if the browser supports notifications and request permission if needed
  requestPermission(): boolean {
    if ('Notification' in window) 
      Notification.requestPermission().then(permission => this.granted = permission === 'granted');
    
    return this.granted;
  }

  // Send a notification if the condition is met
  sendNotification(temp: number): void {
    if (this.granted) {
      const notification = new Notification('High Temperature Alert!', {
        body: `The current temperature is ${temp}°C, which is extremely high.`,
        icon: '/assets/danger.png'
      });
      notification.onclick = () => window.focus();
    } else {
      console.log('No permission to send notifications.');
    }
  }
}
