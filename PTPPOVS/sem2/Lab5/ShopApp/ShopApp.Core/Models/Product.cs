using System.ComponentModel.DataAnnotations;
using Microsoft.EntityFrameworkCore;

namespace ShopApp.Models
{
    /// <summary>
    /// Information about Product
    /// </summary>
    [Index(nameof(Id), IsUnique = true, Name = "IX_Product_Id")]
    public class Product
    {
        /// <summary>
        /// Unique Id in system
        /// </summary>
        [Key]
        public Guid Id { get; set; }

        /// <summary>
        /// Product description from the manufacturer
        /// </summary>
        [Required]
        [MaxLength(500)]    
        public string Description { get; set; }

        /// <summary>
        /// Recommended price from the manufacturer
        /// </summary>
        [Required]
        [Range(0, double.MaxValue)]
        public double Price { get; set; }

        /// <summary>
        /// The date and time when the product was created.
        /// </summary>
        public DateTime CreatedDate { get; set; } = DateTime.UtcNow;
    }
}